// Fill out your copyright notice in the Description page of Project Settings.


#include "FengMao.h"
#include "MonsterAIController.h"
#include "../Particle/ParticleNiagara.h"
#include "../Particle/ParticleCascade.h"
#include "../Particle/RockBurst.h"
#include "../Basic/CollisionObject.h"

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
}

void AFengMao::BeginPlay()
{
	Super::BeginPlay();
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
	
	if (mSkill3SpawnCount == 0)
		GetWorld()->GetTimerManager().SetTimer(mTimerHandle, this, &AFengMao::Skill3, 0.3f, false);
	mSkill3SpawnCount++;
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




	//------------------------
	// ������ ��ġ�� ���� ��, �����Ѵ�.
	//------------------------
	//FVector position = GetActorLocation();
	FVector position = target->GetActorLocation();
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
