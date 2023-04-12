// Fill out your copyright notice in the Description page of Project Settings.


#include "FengMao.h"

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
