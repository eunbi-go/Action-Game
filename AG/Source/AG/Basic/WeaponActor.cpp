// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"

AWeaponActor::AWeaponActor()
{
	mTrail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Trail"));

	mTrail->SetupAttachment(GetMesh());
}

void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();

	UNiagaraSystem* Particle = LoadObject<UNiagaraSystem>(
		nullptr, TEXT("NiagaraSystem'/Game/Assets/Niagara/NS_Ribbon.NS_Ribbon'"));

	if (IsValid(Particle))
		mTrail->SetAsset(Particle);
	mTrail->SetVisibility(true);

	
}
