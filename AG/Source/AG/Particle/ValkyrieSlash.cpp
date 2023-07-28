// Fill out your copyright notice in the Description page of Project Settings.


#include "ValkyrieSlash.h"

void AValkyrieSlash::BeginPlay()
{
	Super::BeginPlay();

	
	GetWorld()->GetTimerManager().SetTimer(mTimer, this, &AValkyrieSlash::Stop, 0.1f, false);
}

void AValkyrieSlash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector location = GetActorLocation();
	location += mDirection * 1000.f * DeltaTime;
	SetActorLocation(location);
}

void AValkyrieSlash::Stop()
{
	mParticle->SetPaused(true);
	GetWorld()->GetTimerManager().SetTimer(mTimer, this, &AValkyrieSlash::Start, 0.2f, false);
}

void AValkyrieSlash::Start()
{
	mParticle->ResetSystem();
}
