// Fill out your copyright notice in the Description page of Project Settings.


#include "FengMao.h"
#include "MonsterAIController.h"
#include "../Particle/ParticleNiagara.h"
#include "../Particle/ParticleCascade.h"
#include "../Particle/RockBurst.h"
#include "../Particle/RampageSlash.h"
#include "../Particle/Meteo.h"
#include "../Basic/CollisionObject.h"
#include "../Particle/Decal.h"
#include "MonsterAnimInstance.h"
#include "../AGGameInstance.h"
#include "../AGGameModeBase.h"
#include "../Widget/MainWidget.h"
#include "../Widget/BossInfoWidget.h"
#include "MonsterSpawnPoint.h"

AFengMao::AFengMao()
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(110.f);
	GetCapsuleComponent()->SetCapsuleRadius(50.f);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -110.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));



	mMonsterTableRowName = TEXT("FengMao");


	mSkillNameArray.Add(TEXT("GroundSmash"));
	mSkillNameArray.Add(TEXT("Energize"));
	mSkillNameArray.Add(TEXT("Roar"));


	mSkill3SpawnCount = 0;


	static ConstructorHelpers::FClassFinder<UCameraShakeBase>	cameraShake(TEXT("Blueprint'/Game/Blueprints/CameraShake/CS_Meteo.CS_Meteo_C'"));

	if (cameraShake.Succeeded())
		mMeteoCameraShake = cameraShake.Class;


	mSkill4Count = 0;
	originalPos = FVector(0.0f);
	isEnableSkill4Respawn = true;

	mSkill1Count = 0;
	isEnableSkill1Respawn = true;
	mSkill1CenterPosition = FVector(0.0f);

	mSkill3Index = 0;
}

void AFengMao::BeginPlay()
{
	Super::BeginPlay();
	mWidgetComopnent->DestroyComponent();
}

void AFengMao::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (mSkill4Count >= 7 && isEnableSkill4Respawn)
	{
		isEnableSkill4Respawn = false;
		mSkill4Count = 0;
	}

	if (mSkill1Count >= 5 && isEnableSkill1Respawn)
	{
		isEnableSkill1Respawn = false;
		mSkill1Count = 0;
	}

	// 플레이어와의 거리에 따라 UI ON / OFF.
	AMonsterAIController* controller = Cast<AMonsterAIController>(GetController());

	if (!IsValid(controller))
		return;

	ACharacter* target = Cast<ACharacter>(controller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (!IsValid(target))
		return;

	FVector	position = GetActorLocation();
	FVector targetPosition = target->GetActorLocation();

	float distance = (position - targetPosition).Size();

	AAGGameModeBase* GameMode = Cast<AAGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	// 알아낸 게임모드가 AAGGameModeBase 가 아니라면 캐스팅 실패 == 현재 월드가 메인 레벨이 아니라는 뜻
	if (nullptr == GameMode)
		return;


	// 현재 게임모드가 AAGGameModeBase 가 맞다면, MainHUD 에 접근해서 InventoryWiget 의 Visible 여부를 확인한다.
	//UMainWidget* MainHUD = GameMode->GetMainWidget();
	//if (distance <= 5000.0f)
	//	MainHUD->BossInfoOnOff(true);
	//else
	//	MainHUD->BossInfoOnOff(false);

	//if (mInfo.hp <= 0)
	//{
	//	MainHUD->BossInfoOnOff(false);
	//}
}

void AFengMao::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AFengMao::UnPossessed()
{
	Super::UnPossessed();
}

float AFengMao::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	int32 damage = (int32)Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	damage -= mInfo.defensePoint;

	if (damage < 1)
		damage = 1;


	mInfo.hp -= damage;

	//PrintViewport(4.f, FColor::Red, FString::Printf(TEXT("maxhp: %d, hp: %d, damage: %d"), mInfo.maxHp, mInfo.hp, damage));

	mInfo.hp < 0.0f ? 0.0f : mInfo.hp;

	AAGGameModeBase* GameMode = Cast<AAGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	// 현재 게임모드가 AAGGameModeBase 가 맞다면, MainHUD 에 접근해서 InventoryWiget 의 Visible 여부를 확인한다.
	UMainWidget* MainHUD = GameMode->GetMainWidget();


	if (IsValid(MainHUD))
	{
		MainHUD->UpdateBossHp(mInfo.hp, mInfo.maxHp);
	}

	if (mInfo.hp <= 0)
	{
		// 다시 충돌되지 않도록.
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		mAnimInst->SetMonsterMotionType(MONSTER_MOTION::DEATH);


		// 동작되고 있던 로직을 멈춘다.
		AAIController* ai = Cast<AAIController>(GetController());

		if (IsValid(ai))
			ai->BrainComponent->StopLogic(TEXT("Death"));

		MainHUD->BossInfoOnOff(false);
		mSpawnPoint->RemoveMonster(this);
	}
	else
	{
		if (mAnimInst->GetMonsterMotionType() != MONSTER_MOTION::SKILL1
			&& mAnimInst->GetMonsterMotionType() != MONSTER_MOTION::SKILL2
			&& mAnimInst->GetMonsterMotionType() != MONSTER_MOTION::SKILL3
			&& mAnimInst->GetMonsterMotionType() != MONSTER_MOTION::SKILL4
			&& mAnimInst->GetMonsterMotionType() != MONSTER_MOTION::SKILL5)
		{
			//---------------------
			// 자기 자신과 DamageCauser 사이의 각도를 구해 각도에 따라 다른 Hit 애니메이션을 재생한다. 
			//---------------------

			FVector targetPosition = DamageCauser->GetActorLocation();
			FVector position = GetActorLocation();
			FVector direction = targetPosition - position;

			direction.Z = 0.f;
			direction.Normalize();

			float innerProduct = FVector::DotProduct(GetActorForwardVector(), direction);
			float degree = UKismetMathLibrary::DegAcos(innerProduct);

			FVector outProduct = FVector::CrossProduct(GetActorForwardVector(), direction);
			float sign = UKismetMathLibrary::SignOfFloat(outProduct.Z);

			float angle = sign * degree;
			FString angleString = TEXT("");

			// 오른쪽.
			if (angle >= 0.f)
			{
				if (degree >= 50.f && angle <= 130.f)
					angleString = TEXT("Right");
				else if (degree < 50.f)
					angleString = TEXT("Front");
				else
					angleString = TEXT("Back");
			}

			// 왼쪽
			else if (angle < 0.f)
			{
				if (degree <= -50.f && angle >= -130.f)
					angleString = TEXT("Left");
				else if (degree > -50.f)
					angleString = TEXT("Front");
				else
					angleString = TEXT("Back");
			}

			mAnimInst->SetHitDirection(angleString);
		}
	}

	return damage;
}

void AFengMao::NormalAttackCheck()
{
	Super::NormalAttackCheck();

	FVector startPosition = GetActorLocation() + GetActorForwardVector() * 10.f;
	FVector endPosition = startPosition + GetActorForwardVector() * mInfo.attackDistance;

	FCollisionQueryParams	param(NAME_None, false, this);

	TArray<FHitResult>	collisionResult;

	bool IsCollision = GetWorld()->SweepMultiByChannel(
		collisionResult, startPosition,
		endPosition, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(mInfo.attackDistance),
		param);


#if ENABLE_DRAW_DEBUG

	//// CollisionEnable 가 true이면 Red, false이면 Green을 저장한다.
	//FColor	DrawColor = IsCollision ? FColor::Red : FColor::Green;

	//// FRotationMatrix::MakeFromZ(GetActorForwardVector()) : 앞쪽을
	//// 바라보는 회전행렬을 만들어서 .ToQuat() 함수를 이용하여 회전행렬을
	//// 회전값으로 변환해준다.
	//DrawDebugCapsule(GetWorld(), (startPosition + endPosition) / 2.f,
	//	mInfo.attackDistance / 2.f,
	//	mInfo.attackDistance,
	//	FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(),
	//	DrawColor, false, 0.5f);

#endif


	if (IsCollision)
	{
		int32	Count = collisionResult.Num();

		for (int32 i = 0; i < Count; ++i)
		{
			//FActorSpawnParameters	SpawnParam;
			////SpawnParam.Template = mHitActor;
			//SpawnParam.SpawnCollisionHandlingOverride =
			//	ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			//// Hit 파티클 렌더링.
			//AParticleCascade* Particle =
			//	GetWorld()->SpawnActor<AParticleCascade>(
			//		collisionResult[i].ImpactPoint,
			//		collisionResult[i].ImpactNormal.Rotation(),
			//		SpawnParam);

			//Particle->SetParticle(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Mobile/Fire/combat/P_Fire_AOE_Blast_mobile.P_Fire_AOE_Blast_mobile'"));

			IHitInterface* hitInterface = Cast<IHitInterface>(collisionResult[i].GetActor());
			if (hitInterface)
			{
				hitInterface->GetHit(collisionResult[i].ImpactPoint);
			}

			// 데미지 계산.
			collisionResult[i].GetActor()->TakeDamage(
				(float)mInfo.attackPoint,
				FDamageEvent(),
				GetController(),
				this);
		}
	}
}

// 슬래시 날림.
void AFengMao::Skill1()
{
	//------------------------
	// 예외처리.
	//------------------------

	AMonsterAIController* aiCotroller = Cast<AMonsterAIController>(GetController());

	ACharacter* target = Cast<ACharacter>(aiCotroller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!IsValid(target))
		return;

	isEnableSkill1Respawn = true;


	FActorSpawnParameters	params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector position = GetActorLocation();
	mSkill1CenterPosition = position;

	ARampageSlash* particle = GetWorld()->SpawnActor<ARampageSlash>(
		position,
		GetActorRotation(),
		params);



	//------------------------
	// 이펙트와 델리게이트를 설정한다.
	//------------------------

	UNiagaraSystem* effect = nullptr;
	int32 effectCount = mSkillInfoArray[mUsingSkillIndex].effectArray.Num();

	for (int32 j = 0; j < effectCount; ++j)
	{
		effect = mSkillInfoArray[mUsingSkillIndex].effectArray[j].niagara;
	}

	particle->SetParticle(effect);
	particle->SetActorScale3D(FVector(0.3f));
	particle->SetActorRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

	particle->SetTarget(target);

	//particle->mReSpawn.AddDynamic(this, &AFengMao::RespawnSkill1);

	particle->mOnHit.AddDynamic(this, &AFengMao::Hit);
	mSkill1MoveStart.AddDynamic(particle, &ARampageSlash::MoveStart);


	mPreSlash = particle;
	// 0.2초 간격으로 순차적으로 생성
	GetWorld()->GetTimerManager().SetTimer(
		mSkillTimerHandle, this, &AFengMao::RespawnSkill1, 0.3f, false
	);

	mSkill1Count++;

}

void AFengMao::RespawnSkill1()
{
	if (!isEnableSkill1Respawn || mSkill1Count >= 5)
	{
		return;
	}

	AMonsterAIController* aiCotroller = Cast<AMonsterAIController>(GetController());

	ACharacter* target = Cast<ACharacter>(aiCotroller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!IsValid(target))
		return;



	//------------------------
	// 스폰할 위치를 정한 후, 스폰한다.
	//------------------------

	FVector direction = GetActorForwardVector();

	direction.Normalize();

	FVector position = mSkill1CenterPosition + direction * 500.f;

	if (mSkill1Count <= 2)
		position.X += 500.0f;
	else
		position.X += 800.0f;

	if (mSkill1Count % 2 == 1)
		position.Y -= 500.0f;
	else
		position.Y += 500.0f;

	FActorSpawnParameters	params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ARampageSlash* particle = GetWorld()->SpawnActor<ARampageSlash>(
		position,
		GetActorRotation(),
		params);



	//------------------------
	// 이펙트와 델리게이트를 설정한다.
	//------------------------

	UNiagaraSystem* effect = nullptr;
	int32 effectCount = mSkillInfoArray[mUsingSkillIndex].effectArray.Num();

	for (int32 j = 0; j < effectCount; ++j)
	{
		effect = mSkillInfoArray[mUsingSkillIndex].effectArray[j].niagara;
	}

	particle->SetParticle(effect);
	particle->SetActorScale3D(FVector(0.3f));
	particle->SetActorRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

	particle->SetTarget(target);

	
	particle->mOnHit.AddDynamic(this, &AFengMao::Hit);
	mSkill1MoveStart.AddDynamic(particle, &ARampageSlash::MoveStart);

	mPreSlash = particle;
	// 0.2초 간격으로 순차적으로 생성
	GetWorld()->GetTimerManager().SetTimer(
		mSkillTimerHandle, this, &AFengMao::RespawnSkill1, 0.3f, false
	);

	mSkill1Count++;

}

void AFengMao::Skill2()
{
}

// 메테오 낙하.
void AFengMao::Skill3()
{
	//------------------------
	// 예외처리.
	//------------------------

	AMonsterAIController* aiCotroller = Cast<AMonsterAIController>(GetController());

	ACharacter* target = Cast<ACharacter>(aiCotroller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!IsValid(target))
		return;



	for (int32 i = 0; i < 3; ++i)
	{
		//------------------------
		// 스폰할 위치를 정한 후, 스폰한다.
		//------------------------

		// 플레이어와 몬스터 사이 각도 구해서 각도의 범위에 따라 나눠야 함.

		//float randomX = FMath::RandRange(-100.0f, -800.0f);
		//float randomY = FMath::RandRange(-800.0f, 800.0f);
		float randomX = FMath::RandRange(100.0f, 800.0f);
		float randomY = FMath::RandRange(-500.0f, 800.0f);

		FVector direction = target->GetActorLocation() - GetActorLocation();

		FVector position = GetActorLocation() + direction.GetSafeNormal2D() * FVector(randomX, randomY, 1.0f);

		position.Z -= (GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2.f);
		
		skill3PositionArray.Add(position);

		FActorSpawnParameters	SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ADecal* decal =
			GetWorld()->SpawnActor<ADecal>(
				position,
				FRotator(0.f, 0.f, 0.f),
				SpawnParam);
		
		decal->SetDecalMaterial(TEXT("Material'/Game/MTMagicCircle.MTMagicCircle'"));
		decal->SetActorScale3D(FVector(0.6f, 0.6f, 0.4f));
		decal->SetLifeSpan(1.f);
		decal->SetDecalVisibility(true);

	}
	
	// 1초 뒤, 데칼 없어지고 나이아가라 생성.

	if (mSkill3SpawnCount == 0)
	{
		GetWorld()->GetTimerManager().SetTimer(mTimerHandle, this, &AFengMao::Skill3, 0.3f, false);
		mSkill3SpawnCount++;
	}
	else
	{
		mSkill3SpawnCount = 0;
		mSkill3Index = 0;
		
		SpawnSkill3();
	}
}

void AFengMao::SpawnSkill3()
{
	AMonsterAIController* aiCotroller = Cast<AMonsterAIController>(GetController());

	ACharacter* target = Cast<ACharacter>(aiCotroller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!IsValid(target))
		return;



	for (int32 i = 0; i < 3; ++i)
	{
		//------------------------
		// 스폰할 위치를 정한 후, 스폰한다.
		//------------------------


		FVector position = skill3PositionArray[mSkill3Index++];
		//position.Z = 0.0f;

		FActorSpawnParameters	params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AMeteo* particle = GetWorld()->SpawnActor<AMeteo>(
			position,
			FRotator::ZeroRotator,
			params);



		//------------------------
		// 이펙트와 델리게이트를 설정한다.
		//------------------------

		UNiagaraSystem* effect = nullptr;
		int32 effectCount = mSkillInfoArray[mUsingSkillIndex].effectArray.Num();

		for (int32 j = 0; j < effectCount; ++j)
		{
			effect = mSkillInfoArray[mUsingSkillIndex].effectArray[j].niagara;
		}

		particle->SetParticle(effect);
		particle->SetActorScale3D(FVector(0.7f));
		particle->mOnHittd.AddDynamic(this, &AFengMao::Temp);
		particle->mCameraShake.AddDynamic(this, &AFengMao::CameraShake);

	}

	// 1초 뒤, 데칼 없어지고 나이아가라 생성.
	if (mSkill3SpawnCount == 0)
	{
		GetWorld()->GetTimerManager().SetTimer(mTimerHandle, this, &AFengMao::SpawnSkill3, 0.1f, false);
		mSkill3SpawnCount++;
	}
	else
	{
		mSkill3SpawnCount = 0;
		mSkill3Index = 0;
		skill3PositionArray.RemoveAll([](FVector) {return true; });
	}

}

void AFengMao::Skill4()
{
	//------------------------
	// 예외처리.
	//------------------------
	AMonsterAIController* aiCotroller = Cast<AMonsterAIController>(GetController());

	ACharacter* target = Cast<ACharacter>(aiCotroller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!IsValid(target))
		return;
	mSkill4Count = 0;

	isEnableSkill4Respawn = true;
	mSkill4Count++;

	//------------------------
	// 스폰할 위치를 정한 후, 스폰한다.
	//------------------------
	FVector position = GetActorLocation();
	position.Z -= (GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2.f);

	originalPos = position;

	FActorSpawnParameters	params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ARockBurst* particle = GetWorld()->SpawnActor<ARockBurst>(
		position,
		FRotator::ZeroRotator,
		params);



	//------------------------
	// 이펙트와 델리게이트를 설정한다.
	//------------------------

	UParticleSystem* effect = nullptr;
	int32 effectCount = mSkillInfoArray[mUsingSkillIndex].effectArray.Num();

	for (int32 j = 0; j < effectCount; ++j)
	{
		effect = mSkillInfoArray[mUsingSkillIndex].effectArray[j].particle;
	}

	particle->SetParticle(effect);
	particle->SetActorScale3D(FVector(0.5f));

	particle->mReSpawn.AddDynamic(this, &AFengMao::RespawnSkill4);
	particle->mOnHit.AddDynamic(this, &AFengMao::Hit);
}

void AFengMao::SkillCollisionCheck(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

}



void AFengMao::Temp(ACollisionObject* collisionObject, const FHitResult& Hit, AActor* hitActor)
{
	IHitInterface* hitInterface = Cast<IHitInterface>(hitActor);
	if (hitInterface)
	{
		hitInterface->GetHit(Hit.ImpactPoint);
	}
	hitActor->TakeDamage(100.f, FDamageEvent(), GetController(), this);
	collisionObject->Destroy();
}

void AFengMao::CameraShake(AMeteo* niagara)
{
	GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(mMeteoCameraShake);
}

void AFengMao::RespawnSkill4(ARockBurst* particles)
{
	if (!isEnableSkill4Respawn)
	{
		return;
	}

	AMonsterAIController* aiCotroller = Cast<AMonsterAIController>(GetController());

	ACharacter* target = Cast<ACharacter>(aiCotroller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!IsValid(target))
		return;



	//------------------------
	// 스폰할 위치를 정한 후, 스폰한다.
	//------------------------
	//FVector position = GetActorLocation();

	for (int32 i = 0; i < 3; ++i)
	{
		FVector direction = GetActorForwardVector();
		
		if (i == 1)
		{
			FVector dir = GetActorRightVector();
			dir.Y *= -1.0f;
			dir += GetActorForwardVector();
			direction += dir;
		}
		else if (i == 2)
		{
			direction += GetActorRightVector();
		}

		direction.Normalize();

		FVector position = particles->GetActorLocation() + direction * 200.f;
		position.Z -= (GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2.f);

		FActorSpawnParameters	params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ARockBurst* particle = GetWorld()->SpawnActor<ARockBurst>(
			position,
			FRotator::ZeroRotator,
			params);



		//------------------------
		// 이펙트와 델리게이트를 설정한다.
		//------------------------

		UParticleSystem* effect = nullptr;
		int32 effectCount = mSkillInfoArray[mUsingSkillIndex].effectArray.Num();

		for (int32 j = 0; j < effectCount; ++j)
		{
			effect = mSkillInfoArray[mUsingSkillIndex].effectArray[j].particle;
		}

		particle->SetParticle(effect);
		particle->SetActorScale3D(FVector(0.5f));

		particle->mReSpawn.AddDynamic(this, &AFengMao::RespawnSkill4);
		particle->mOnHit.AddDynamic(this, &AFengMao::Hit);
	}

	mSkill4Count++;

	particles->Destroy();
}




void AFengMao::Hit(FVector impactPoint)
{
	AMonsterAIController* aiCotroller = Cast<AMonsterAIController>(GetController());

	ACharacter* target = Cast<ACharacter>(aiCotroller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (IsValid(target))
	{
		IHitInterface* hitInterface = Cast<IHitInterface>(target);
		if (hitInterface)
		{
			hitInterface->GetHit(impactPoint);
		}
		target->TakeDamage(100.f, FDamageEvent(), GetController(), this);
	}
}

void AFengMao::StartJump()
{
	GetCharacterMovement()->BrakingFrictionFactor = 0.f;
	LaunchCharacter(FVector(0.0f, 0.0f, 1.0f) * 1000.0f, true, true);
	GetWorld()->GetTimerManager().SetTimer(mTimerHandle, this, &AFengMao::StopJump, 0.3f, false);
}

void AFengMao::StopJump()
{
	GetCharacterMovement()->BrakingFrictionFactor = 2.f;
	GetCharacterMovement()->StopMovementImmediately();

	Skill1();
}
