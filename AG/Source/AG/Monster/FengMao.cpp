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
#include "../Widget/HUD/AGHUD.h"
#include "../AbilitySystem/AGAttributeSet.h"

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


	UAnimMontage* montage;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> equipMontage(
		TEXT("AnimMontage'/Game/Blueprints/Monster/Animation/Rampage/AM_Rampage_Energize.AM_Rampage_Energize'")
	);
	if (equipMontage.Succeeded())
	{
		montage = equipMontage.Object;
	}
	mMontages.Add(FName("Energize"), montage);

	UAnimMontage* montage2;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> equipMontage2(
		TEXT("AnimMontage'/Game/Blueprints/Monster/Animation/Rampage/AM_Rampage_Roar.AM_Rampage_Roar'")
	);
	if (equipMontage2.Succeeded())
	{
		montage2 = equipMontage2.Object;
	}
	mMontages.Add(FName("Roar"), montage2);

	UAnimMontage* montage3;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> equipMontage3(
		TEXT("AnimMontage'/Game/Blueprints/Monster/Animation/Rampage/AM_Rampage_Smash.AM_Rampage_Smash'")
	);
	if (equipMontage3.Succeeded())
	{
		montage3 = equipMontage3.Object;
	}
	mMontages.Add(FName("Stone"), montage3);
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

	// �÷��̾���� �Ÿ��� ���� UI ON / OFF.
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

	// �˾Ƴ� ���Ӹ�尡 AAGGameModeBase �� �ƴ϶�� ĳ���� ���� == ���� ���尡 ���� ������ �ƴ϶�� ��
	if (nullptr == GameMode)
		return;


	// ���� ���Ӹ�尡 AAGGameModeBase �� �´ٸ�, MainHUD �� �����ؼ� InventoryWiget �� Visible ���θ� Ȯ���Ѵ�.
	AAGHUD* hud = Cast<AAGHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (!IsValid(hud))
		return;
	UMainWidget* mainWidget = hud->mMainWidget;
	if (!IsValid(mainWidget))
		return;

	if (distance <= 5000.0f)
		mainWidget->BossInfoOnOff(true);
	else
		mainWidget->BossInfoOnOff(false);

	if (Cast<UAGAttributeSet>(mAttributeSet)->GetmHp() <= 0)
	{
		mainWidget->BossInfoOnOff(false);
	}
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
	int32 damage = DamageAmount;
	//int32 damage = (int32)Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	damage -= Cast<UAGAttributeSet>(mAttributeSet)->GetmDefense();
	if (damage < 1)
		damage = 1;

	int32 randomValue = FMath::RandRange(10, 20);
	damage -= randomValue;
	damage = fabsf(damage);
	//Cast<UAGAttributeSet>(mAttributeSet)->SetmHp(Cast<UAGAttributeSet>(mAttributeSet)->GetmHp() - damage);

	AAGGameModeBase* GameMode = Cast<AAGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	AAGHUD* hud = Cast<AAGHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (!IsValid(hud))
		return damage;
	UMainWidget* mainWidget = hud->mMainWidget;
	if (!IsValid(mainWidget))
		return damage;

	mainWidget->UpdateBossHp((float)Cast<UAGAttributeSet>(mAttributeSet)->GetmHp(), Cast<UAGAttributeSet>(mAttributeSet)->GetmMaxHp());

	if (Cast<UAGAttributeSet>(mAttributeSet)->GetmHp() <= 0)
	{
		// �ٽ� �浹���� �ʵ���.
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		mAnimInst->SetMonsterMotionType(MONSTER_MOTION::DEATH);


		// ���۵ǰ� �ִ� ������ �����.
		AAIController* ai = Cast<AAIController>(GetController());
		if (IsValid(ai))
			ai->BrainComponent->StopLogic(TEXT("Death"));

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
			// �ڱ� �ڽŰ� DamageCauser ������ ������ ���� ������ ���� �ٸ� Hit �ִϸ��̼��� ����Ѵ�. 
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

			// ������.
			if (angle >= 0.f)
			{
				if (degree >= 50.f && angle <= 130.f)
					angleString = TEXT("Right");
				else if (degree < 50.f)
					angleString = TEXT("Front");
				else
					angleString = TEXT("Back");
			}

			// ����
			else if (angle < 0.f)
			{
				if (degree <= -50.f && angle >= -130.f)
					angleString = TEXT("Left");
				else if (degree > -50.f)
					angleString = TEXT("Front");
				else
					angleString = TEXT("Back");
			}

			//mAnimInst->SetHitDirection(angleString);
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

	//// CollisionEnable �� true�̸� Red, false�̸� Green�� �����Ѵ�.
	//FColor	DrawColor = IsCollision ? FColor::Red : FColor::Green;

	//// FRotationMatrix::MakeFromZ(GetActorForwardVector()) : ������
	//// �ٶ󺸴� ȸ������� ���� .ToQuat() �Լ��� �̿��Ͽ� ȸ�������
	//// ȸ�������� ��ȯ���ش�.
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

			//// Hit ��ƼŬ ������.
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

			// ������ ���.
			collisionResult[i].GetActor()->TakeDamage(
				(float)mInfo.attackPoint,
				FDamageEvent(),
				GetController(),
				this);
		}
	}
}

// ������ ����.
void AFengMao::Skill1()
{
	//------------------------
	// ����ó��.
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
	mSkill1TargetPosition = target->GetActorLocation();

	ARampageSlash* particle = GetWorld()->SpawnActor<ARampageSlash>(
		position,
		GetActorRotation(),
		params);



	//------------------------
	// ����Ʈ�� ��������Ʈ�� �����Ѵ�.
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
	// 0.2�� �������� ���������� ����
	GetWorld()->GetTimerManager().SetTimer(
		mSkillTimerHandle, this, &AFengMao::RespawnSkill1, 0.3f, false
	);

	mSkill1Count++;

}

void AFengMao::RespawnSkill1()
{
	if (!isEnableSkill1Respawn || mSkill1Count == 5)
	{
		return;
	}

	AMonsterAIController* aiCotroller = Cast<AMonsterAIController>(GetController());

	ACharacter* target = Cast<ACharacter>(aiCotroller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!IsValid(target))
		return;



	//------------------------
	// ������ ��ġ�� ���� ��, �����Ѵ�.
	//------------------------

	FVector forward = GetActorForwardVector();
	FVector targetRight = target->GetActorRightVector().GetSafeNormal();
	FVector direction = (target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	FVector position = FVector(0.f);

	if (mSkill1Count == 1)
	{
		position = target->GetActorLocation() + targetRight * 500.f;
	}
	else if (mSkill1Count == 2)
	{
		float disToTarget = GetDistanceTo(target);
		disToTarget /= 2.f;
		position = GetActorLocation() + direction * disToTarget;
		position += targetRight * 250.f;
	}
	else if (mSkill1Count == 3)
	{
		position = target->GetActorLocation() - targetRight * 500.f;
	}
	else if (mSkill1Count == 4)
	{
		float disToTarget = GetDistanceTo(target);
		disToTarget /= 2.f;
		position = GetActorLocation() + direction * disToTarget;
		position -= targetRight * 250.f;
	}
	position.Z = GetActorLocation().Z + GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.f;

	FActorSpawnParameters	params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ARampageSlash* particle = GetWorld()->SpawnActor<ARampageSlash>(
		position,
		GetActorRotation(),
		params);



	//------------------------
	// ����Ʈ�� ��������Ʈ�� �����Ѵ�.
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
	// 0.2�� �������� ���������� ����
	GetWorld()->GetTimerManager().SetTimer(
		mSkillTimerHandle, this, &AFengMao::RespawnSkill1, 0.3f, false
	);

	mSkill1Count++;

}

void AFengMao::Skill2()
{
}

// ���׿� ����.
void AFengMao::Skill3()
{
	//------------------------
	// ����ó��.
	//------------------------

	AMonsterAIController* aiCotroller = Cast<AMonsterAIController>(GetController());
	ACharacter* target = Cast<ACharacter>(aiCotroller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (!IsValid(target))
		return;

	FVector location = GetActorLocation();
	FVector targetLocation = target->GetActorLocation();
	float minX = (targetLocation + target->GetActorRightVector().GetSafeNormal() * mMeteoRangeWidth).X;
	float maxX = (targetLocation - target->GetActorRightVector().GetSafeNormal() * mMeteoRangeWidth).X;
	float maxY = location.Y;
	float minY = targetLocation.Y;
	
	for (int32 i = 0; i < 3; ++i)
	{
		//------------------------
		// ������ ��ġ�� ���� ��, �����Ѵ�.
		//------------------------

		float randomX = FMath::RandRange(fminf(minX, maxX), fmaxf(minX, maxX));
		float randomY = FMath::RandRange(fminf(maxY, minY), fmaxf(maxY, minY));
		FVector position = FVector(randomX, randomY, location.Z);
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
	
	// 1�� ��, ��Į �������� ���̾ư��� ����.

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
		// ������ ��ġ�� ���� ��, �����Ѵ�.
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
		// ����Ʈ�� ��������Ʈ�� �����Ѵ�.
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

	// 1�� ��, ��Į �������� ���̾ư��� ����.
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

// ��
void AFengMao::Skill4()
{
	//------------------------
	// ����ó��.
	//------------------------
	AMonsterAIController* aiCotroller = Cast<AMonsterAIController>(GetController());

	ACharacter* target = Cast<ACharacter>(aiCotroller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!IsValid(target))
		return;
	mSkill4Count = 0;

	isEnableSkill4Respawn = true;
	mSkill4Count++;

	//------------------------
	// ������ ��ġ�� ���� ��, �����Ѵ�.
	//------------------------
	FVector position = GetActorLocation();
	position.Z += (GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2.f);

	originalPos = position;

	FActorSpawnParameters	params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ARockBurst* particle = GetWorld()->SpawnActor<ARockBurst>(
		position,
		FRotator::ZeroRotator,
		params);



	//------------------------
	// ����Ʈ�� ��������Ʈ�� �����Ѵ�.
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
	// ������ ��ġ�� ���� ��, �����Ѵ�.
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
		// ����Ʈ�� ��������Ʈ�� �����Ѵ�.
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


void AFengMao::SkillCollisionCheck(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

}

void AFengMao::PlaySkillMontage(MONSTER_MOTION motion)
{
	UAnimMontage* montage = nullptr;
	if (motion == MONSTER_MOTION::SKILL1)
	{
		montage = *mMontages.Find("Stone");
	}
	else if (motion == MONSTER_MOTION::SKILL2)
	{
		montage = *mMontages.Find("Stone");
	}
	else if (motion == MONSTER_MOTION::SKILL3)
	{
		montage = *mMontages.Find("Energize");
	}
	else if (motion == MONSTER_MOTION::SKILL4)
	{
		montage = *mMontages.Find("Roar");
	}
	else
		return;
	mAnimInst->Montage_Play(montage);
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
