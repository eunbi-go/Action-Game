// Fill out your copyright notice in the Description page of Project Settings.


#include "ValkyrieSprintEffect.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AValkyrieSprintEffect::AValkyrieSprintEffect()
{
	PrimaryActorTick.bCanEverTick = false;
	/*
	* ������ �ƴ� "����"���� �߻�ü�� �����Ѵ�.
	* �߻�ü�� replicated actor �� �Ǳ⸦ ���Ѵ�.
	* - ������ �߻�ü�� �����ϸ� ������ �߻�ü �̵�, ��ġ ó�� �� ��Ÿ ���� �۾��� ����ϰ� �Ǹ�
	*   Ŭ���̾�Ʈ�� �߻�ü�� ������ ������ ���� �ȴ�.
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

		// �������� ��ü ����
		Destroy();
	}
	else
	{
		mIsOverlap = true;
	}
}
