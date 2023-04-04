// Fill out your copyright notice in the Description page of Project Settings.


#include "SprintSkil.h"
#include "../Player/WarriorCharacter.h"

ASprintSkil::ASprintSkil()
{
}

void ASprintSkil::BeginPlay()
{
	Super::BeginPlay();

	mNiagara->OnSystemFinished.AddDynamic(this, &ASprintSkil::Finish);
}

void ASprintSkil::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ASprintSkil::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}

void ASprintSkil::Finish(UNiagaraComponent* comp)
{
	PrintViewport(10.f, FColor::Yellow, TEXT("ASprintSkil::Finish"));
	mOnNiagaraEnd.Broadcast(this, comp);
}
