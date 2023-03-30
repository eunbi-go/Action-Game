// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"

AWeaponActor::AWeaponActor()
{
	mTrail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Trail"));

	mTrail->SetupAttachment(GetMesh());

	mGauge = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Gauge"));

	mGauge->SetupAttachment(GetMesh());
}

void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();

	UNiagaraSystem* Particle = LoadObject<UNiagaraSystem>(
		nullptr, TEXT("NiagaraSystem'/Game/Assets/Niagara/NS_Ribbon.NS_Ribbon'"));

	if (IsValid(Particle))
		mTrail->SetAsset(Particle);
	mTrail->SetVisibility(false);

	mTrail->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
	


	UNiagaraSystem* gauge = LoadObject<UNiagaraSystem>(
		nullptr, TEXT("NiagaraSystem'/Game/Assets/Niagara/NS_Dash2.NS_Dash2'"));

	if (IsValid(gauge))
		mGauge->SetAsset(gauge);
	mGauge->SetVisibility(false);
}
