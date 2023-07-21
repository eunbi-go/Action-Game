// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"

AWeaponActor::AWeaponActor()
{
	mTrail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Trail"));

	mTrail->SetupAttachment(GetMesh());

	mGauge = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Gauge"));

	mGauge->SetupAttachment(GetMesh());
	//mBox->SetCollisionProfileName(TEXT("Weapon"));
	//mBox->SetGenerateOverlapEvents(true);
	//mBox->SetNotifyRigidBodyCollision(true);

	mMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//mBox->SetBoxExtent(FVector(10.f, 10.f, 100.f));

	mBox->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
}

void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();

	UNiagaraSystem* Particle = LoadObject<UNiagaraSystem>(
		nullptr, TEXT("NiagaraSystem'/Game/sA_SwordTrailSet/Fx/NS_Flameslash_3.NS_Flameslash_3'"));

	if (IsValid(Particle))
		mTrail->SetAsset(Particle);
	mTrail->SetVisibility(false);

	mTrail->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
	

	
	/*UNiagaraSystem* gauge = LoadObject<UNiagaraSystem>(
		nullptr, TEXT("NiagaraSystem'/Game/Assets/Niagara/NS_Dash2.NS_Dash2'"));*/
	UNiagaraSystem * gauge = LoadObject<UNiagaraSystem>(
		nullptr, TEXT("NiagaraSystem'/Game/BlinkAndDashVFX/VFX_Niagara/NS_Dash_Fire.NS_Dash_Fire'"));

	if (IsValid(gauge))
		mGauge->SetAsset(gauge);
	mGauge->SetVisibility(false);
}
