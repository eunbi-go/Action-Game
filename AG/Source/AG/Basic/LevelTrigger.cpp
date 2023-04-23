// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTrigger.h"

ALevelTrigger::ALevelTrigger()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALevelTrigger::TriggerBegin(const FHitResult& SweepResult)
{
	PrintViewport(10.f, FColor::Red, TEXT("level trigger"));
	UGameplayStatics::OpenLevel(GetWorld(), mLevelName);
}

void ALevelTrigger::TriggerEnd()
{
}
