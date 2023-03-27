// Fill out your copyright notice in the Description page of Project Settings.


#include "AGPlayerController.h"

AAGPlayerController::AAGPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAGPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AAGPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AAGPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);


}

void AAGPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AAGPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
