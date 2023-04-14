// Fill out your copyright notice in the Description page of Project Settings.


#include "RampageSlash.h"

ARampageSlash::ARampageSlash()
{
}

void ARampageSlash::Stop()
{
	mParticle->SetPaused(true);
	GetWorld()->GetTimerManager().SetTimer(mTimerHandle2, this, &ARampageSlash::Start, 0.3f, false);
}

void ARampageSlash::Start()
{
	mParticle->ResetSystem();
}

void ARampageSlash::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(mTimerHandle2, this, &ARampageSlash::Stop, 0.1f, false);
}