// Fill out your copyright notice in the Description page of Project Settings.


#include "FengMao.h"
#include "MonsterAIController.h"
#include "../Particle/ParticleNiagara.h"
#include "../Particle/ParticleCascade.h"
#include "../Particle/RockBurst.h"
#include "../Particle/RampageSlash.h"
#include "../Basic/CollisionObject.h"
#include "MonsterAnimInstance.h"




AFengMao::AFengMao()
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(110.f);
	GetCapsuleComponent()->SetCapsuleRadius(34.f);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -110.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));



	mMonsterTableRowName = TEXT("FengMao");


	mSkillNameArray.Add(TEXT("GroundSmash"));
	mSkillNameArray.Add(TEXT("Stone"));
	mSkillNameArray.Add(TEXT("Energize"));
	mSkillNameArray.Add(TEXT("Roar"));


	mSkill3SpawnCount = 0;


	static ConstructorHelpers::FClassFinder<UCameraShakeBase>	cameraShake(TEXT("Blueprint'/Game/Blueprints/CameraShake/CS_Meteo.CS_Meteo_C'"));

	if (cameraShake.Succeeded())
		mMeteoCameraShake = cameraShake.Class;


	mSkill4Count = 0;
	originalPos = FVector(0.0f);
	isEnableSkill4Respawn = true;
}

void AFengMao::BeginPlay()
{
	Super::BeginPlay();
}

void AFengMao::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (mSkill4Count >= 7 && isEnableSkill4Respawn)
	{
		isEnableSkill4Respawn = false;
		mSkill4Count = 0;
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

void AFengMao::Skill1()
{
	//------------------------
	// 예외처리.
	//------------------------

	AMonsterAIController* aiCotroller = Cast<AMonsterAIController>(GetController());

	ACharacter* target = Cast<ACharacter>(aiCotroller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!IsValid(target))
		return;



	for (int32 i = 0; i < 6; ++i)
	{
		FActorSpawnParameters	params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector position = GetActorLocation();
		if (i < 4)
		{
			position.Y -= (i * 50.f);
		}
		else
		{
			position.Y += (i * 50.f);
		}

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
	}
}

void AFengMao::Skill2()
{
}

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

		float randomX = FMath::RandRange(-100.0f, -800.0f);
		float randomY = FMath::RandRange(-800.0f, 800.0f);



		FVector position = GetActorLocation() + GetActorForwardVector().Normalize() * FVector(randomX, randomY, 1.0f);
		position.Z = 0.0f;

		FActorSpawnParameters	params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AParticleNiagara* particle = GetWorld()->SpawnActor<AParticleNiagara>(
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
	
	if (mSkill3SpawnCount == 0)
		GetWorld()->GetTimerManager().SetTimer(mTimerHandle, this, &AFengMao::Skill3, 0.3f, false);
	mSkill3SpawnCount++;
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
	position.Z = 0.0f;
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
		position.Z = 0.0f;

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
