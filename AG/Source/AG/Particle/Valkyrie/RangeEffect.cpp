// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeEffect.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

ARangeEffect::ARangeEffect()
{
	PrimaryActorTick.bCanEverTick = false;

	mBox = CreateDefaultSubobject<UBoxComponent>("BoxComp");
	mBox->SetupAttachment(GetRootComponent());
	mBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	mBox->SetCollisionProfileName(FName("PlayerAttack"));

}

void ARangeEffect::Active()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, mParticle->GetAsset(), GetActorLocation());
}

void ARangeEffect::BeginPlay()
{
	Super::BeginPlay();

	SetReplicateMovement(true);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, mParticle->GetAsset(), GetActorLocation());
	mBox->OnComponentBeginOverlap.AddDynamic(this, &ARangeEffect::OnBoxOverlap);
}

void ARangeEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARangeEffect::Destroyed()
{
	if (!mIsOverlap && !HasAuthority())
	{
		mBox->DestroyComponent();
		//UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, mImpactEffect, GetActorLocation());
	}
	Super::Destroyed();
}

void ARangeEffect::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, mImpactEffect, GetActorLocation());

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			targetASC->ApplyGameplayEffectSpecToSelf(*mDamageEffectSpecHandle.Data.Get());
		}

		// 서버에서 객체 삭제
		Destroy();
	}
	else
	{
		mIsOverlap = true;
	}
}
