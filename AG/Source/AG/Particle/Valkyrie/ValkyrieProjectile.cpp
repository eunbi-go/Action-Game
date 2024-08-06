// Fill out your copyright notice in the Description page of Project Settings.


#include "ValkyrieProjectile.h"
#include "../../AbilitySystem/AGAbilitySystemLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

AValkyrieProjectile::AValkyrieProjectile()
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
	mSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	mSphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	mSphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	mSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	mSphere->SetCollisionProfileName(FName("PlayerSword"));


	mProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile");
	mProjectileMovement->InitialSpeed = 550.f;
	mProjectileMovement->MaxSpeed = 550.f;
	mProjectileMovement->ProjectileGravityScale = 0.f;

}

void AValkyrieProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
	
	SetLifeSpan(mLifeSpan);
	mSphere->OnComponentBeginOverlap.AddDynamic(this, &AValkyrieProjectile::OnSphereOverlap);
	
	//mLoopingSoundComp = UGameplayStatics::SpawnSoundAttached(mLoopingSound, GetRootComponent());
}

void AValkyrieProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AValkyrieProjectile::Destroyed()
{
	if (!mIsOverlap && !HasAuthority())
	{
		UGameplayStatics::PlaySoundAtLocation(this, mImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, mImpactEffect, GetActorLocation());
		//mLoopingSoundComp->Stop();
	}
	Super::Destroyed();

}

void AValkyrieProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::PlaySoundAtLocation(this, mImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, mImpactEffect, GetActorLocation());
	//mLoopingSoundComp->Stop();

	if (HasAuthority())
	{
		// UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor)
		
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
