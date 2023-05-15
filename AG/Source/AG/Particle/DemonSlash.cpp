// Fill out your copyright notice in the Description page of Project Settings.


#include "DemonSlash.h"

ADemonSlash::ADemonSlash()
{
	mParticle->SetGenerateOverlapEvents(true);
	mParticle->SetVisibility(true);
}

void ADemonSlash::BeginPlay()
{
	Super::BeginPlay();
	SetParticle(TEXT("NiagaraSystem'/Game/Hack_And_Slash_FX/VFX_Niagara/Slashes/NS_Demon_Slash.NS_Demon_Slash'"));
	mParticle->OnSystemFinished.AddDynamic(this, &ADemonSlash::ParticleFinish);
	mParticle->OnComponentHit.AddDynamic(this, &ADemonSlash::ParticleHit);
}

void ADemonSlash::ParticleFinish(UNiagaraComponent* _particle)
{
	Destroy();
}

void ADemonSlash::ParticleHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//mHitDelegate.Broadcast(OtherActor, Hit);
}
