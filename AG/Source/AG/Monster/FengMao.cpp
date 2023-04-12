// Fill out your copyright notice in the Description page of Project Settings.


#include "FengMao.h"
#include "MonsterAIController.h"
#include "../Particle/ParticleNiagara.h"


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
	AMonsterAIController* aiCotroller = Cast<AMonsterAIController>(GetController());

	ACharacter* target = Cast<ACharacter>(aiCotroller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!IsValid(target))
		return;


	FActorSpawnParameters	params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	float randomX = FMath::RandRange(50.0f, 200.0f);
	float randomY = FMath::RandRange(50.0f, 200.0f);

	FVector position = GetActorLocation() - FVector(randomX, randomY, GetActorLocation().Z /*+ 100.f*/);
	FVector direction = GetActorLocation() - target->GetActorLocation();
	direction.Normalize();
	

	AParticleNiagara* particle = GetWorld()->SpawnActor<AParticleNiagara>(
									position,
									direction.Rotation(),
									params);


	UNiagaraSystem* effect = nullptr;
	int32 effectCount = mSkillInfoArray[mUsingSkillIndex].effectArray.Num();

	for (int32 i = 0; i < effectCount; ++i)
	{
		effect = mSkillInfoArray[mUsingSkillIndex].effectArray[i].niagara;
	}

	particle->SetParticle(effect);
	particle->GetNiagara()->SetRelativeScale3D(FVector(0.7f));
}

void AFengMao::Skill4()
{
}
