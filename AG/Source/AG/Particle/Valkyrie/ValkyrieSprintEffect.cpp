// Fill out your copyright notice in the Description page of Project Settings.


#include "ValkyrieSprintEffect.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AValkyrieSprintEffect::AValkyrieSprintEffect()
{
	PrimaryActorTick.bCanEverTick = false;
	/*
	* 로컬이 아닌 "서버"에서 발사체를 생성한다.
	* 발사체가 replicated actor 가 되기를 원한다.
	* - 서버가 발사체를 생성하면 서버는 발사체 이동, 위치 처리 및 기타 좋은 작업을 담당하게 되며
	*   클라이언트는 발사체의 복제된 버전을 보게 된다.
	*
	*/


	mSphere = CreateDefaultSubobject<USphereComponent>("SphereComp");
	mSphere->SetupAttachment(GetRootComponent());
	mSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	mSphere->SetCollisionProfileName(FName("PlayerSword"));

	mParticle->bAutoActivate = true;
}

void AValkyrieSprintEffect::BeginPlay()
{
	Super::BeginPlay();
	
	SetReplicateMovement(true);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, mParticle->GetAsset(), GetActorLocation());
	mSphere->OnComponentBeginOverlap.AddDynamic(this, &AValkyrieSprintEffect::OnSphereOverlap);
}

void AValkyrieSprintEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AValkyrieSprintEffect::Destroyed()
{
	if (!mIsOverlap && !HasAuthority())
	{
		mSphere->DestroyComponent();
		//UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, mImpactEffect, GetActorLocation());
	}
	Super::Destroyed();
}

void AValkyrieSprintEffect::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UGameplayStatics::PlaySoundAtLocation(this, mImpactSound, GetActorLocation(), FRotator::ZeroRotator);
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
