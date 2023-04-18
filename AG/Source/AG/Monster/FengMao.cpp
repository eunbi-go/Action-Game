// Fill out your copyright notice in the Description page of Project Settings.


#include "FengMao.h"
#include "MonsterAIController.h"
#include "../Particle/ParticleNiagara.h"
#include "../Particle/ParticleCascade.h"
#include "../Particle/RockBurst.h"
#include "../Particle/RampageSlash.h"
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
	GetCapsuleComponent()->SetCapsuleRadius(34.f);

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
	UMainWidget* MainHUD = GameMode->GetMainWidget();
	if (distance <= 5000.0f)
		MainHUD->BossInfoOnOff(true);
	else
		MainHUD->BossInfoOnOff(false);
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

	PrintViewport(4.f, FColor::Red, FString::Printf(TEXT("maxhp: %d, hp: %d, damage: %d"), mInfo.maxHp, mInfo.hp, damage));

	mInfo.hp < 0.0f ? 0.0f : mInfo.hp;

	AAGGameModeBase* GameMode = Cast<AAGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	// ���� ���Ӹ�尡 AAGGameModeBase �� �´ٸ�, MainHUD �� �����ؼ� InventoryWiget �� Visible ���θ� Ȯ���Ѵ�.
	UMainWidget* MainHUD = GameMode->GetMainWidget();


	if (IsValid(MainHUD))
	{
		MainHUD->UpdateBossHp(mInfo.hp, mInfo.maxHp);
	}

	if (mInfo.hp <= 0)
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

		mAnimInst->SetHitDirection(angleString);
	}

	return damage;
}

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
	particle->mReSpawn.AddDynamic(this, &AFengMao::RespawnSkill1);
	particle->mOnHit.AddDynamic(this, &AFengMao::Hit);
	mSkill1Start.AddDynamic(particle, &ARampageSlash::ResetSlash);
	mSkill1MoveStart.AddDynamic(particle, &ARampageSlash::MoveStart);
	mSkill1Count++;

}

void AFengMao::Skill2()
{
}

void AFengMao::Skill3()
{
	//------------------------
	// ����ó��.
	//------------------------

	AMonsterAIController* aiCotroller = Cast<AMonsterAIController>(GetController());

	ACharacter* target = Cast<ACharacter>(aiCotroller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!IsValid(target))
		return;



	for (int32 i = 0; i < 3; ++i)
	{
		//------------------------
		// ������ ��ġ�� ���� ��, �����Ѵ�.
		//------------------------

		float randomX = FMath::RandRange(-100.0f, -800.0f);
		float randomY = FMath::RandRange(-800.0f, 800.0f);



		FVector position = GetActorLocation() + GetActorForwardVector().Normalize() * FVector(randomX, randomY, 1.0f);
		position.Z = 0.f;
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
		position.Z = 0.0f;

		FActorSpawnParameters	params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AParticleNiagara* particle = GetWorld()->SpawnActor<AParticleNiagara>(
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
	position.Z = 0.0f;
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

void AFengMao::SkillCollisionCheck(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

}



void AFengMao::Temp(ACollisionObject* collisionObject, const FHitResult& Hit, AActor* hitActor)
{
	hitActor->TakeDamage(100.f, FDamageEvent(), GetController(), this);
	collisionObject->Destroy();
}

void AFengMao::CameraShake(AParticleNiagara* niagara)
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
		position.Z = 0.0f;

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

void AFengMao::RespawnSkill1(ARampageSlash* preParticle)
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
	// ������ ��ġ�� ���� ��, �����Ѵ�.
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
	particle->mReSpawn.AddDynamic(this, &AFengMao::RespawnSkill1);
	particle->mOnHit.AddDynamic(this, &AFengMao::Hit);
	mSkill1Start.AddDynamic(particle, &ARampageSlash::ResetSlash);
	mSkill1MoveStart.AddDynamic(particle, &ARampageSlash::MoveStart);

	mSkill1Count++;

}


void AFengMao::Hit()
{
	AMonsterAIController* aiCotroller = Cast<AMonsterAIController>(GetController());

	ACharacter* target = Cast<ACharacter>(aiCotroller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (IsValid(target))
	{
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
