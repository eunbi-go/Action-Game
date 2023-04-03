// Fill out your copyright notice in the Description page of Project Settings.


#include "SprintSkil.h"
#include "../Player/WarriorCharacter.h"

ASprintSkil::ASprintSkil()
{
}

void ASprintSkil::BeginPlay()
{
	Super::BeginPlay();
	SetActorScale3D(FVector(1.f, 1.f, 2.f));
}

void ASprintSkil::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ASprintSkil::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}
