// Fill out your copyright notice in the Description page of Project Settings.


#include "ValkyrieFallingSwordEffect.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AValkyrieFallingSwordEffect::AValkyrieFallingSwordEffect()
{
	PrimaryActorTick.bCanEverTick = false;

	mBox = CreateDefaultSubobject<UBoxComponent>("BoxComp");
	mBox->SetupAttachment(GetRootComponent());
	mBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	mBox->SetCollisionProfileName(FName("PlayerAttack"));

	mParticle->bAutoActivate = true;
}

void AValkyrieFallingSwordEffect::BeginPlay()
{
	Super::BeginPlay();

	SetReplicateMovement(true);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, mParticle->GetAsset(), GetActorLocation());
	mBox->OnComponentBeginOverlap.AddDynamic(this, &AValkyrieFallingSwordEffect::OnBoxOverlap);
}

void AValkyrieFallingSwordEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AValkyrieFallingSwordEffect::Destroyed()
{
	if (!mIsOverlap && !HasAuthority())
	{
		mBox->DestroyComponent();
		//UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, mImpactEffect, GetActorLocation());
	}
	Super::Destroyed();
}

void AValkyrieFallingSwordEffect::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
