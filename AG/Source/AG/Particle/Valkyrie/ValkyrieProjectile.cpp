// Fill out your copyright notice in the Description page of Project Settings.


#include "ValkyrieProjectile.h"
#include "../../AbilitySystem/AGAbilitySystemLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

AValkyrieProjectile::AValkyrieProjectile()
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
	mSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	mSphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	mSphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	mSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	mSphere->SetCollisionProfileName(FName("PlayerSword"));


	mProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile");
	mProjectileMovement->InitialSpeed = 900.f;
	mProjectileMovement->MaxSpeed = 900.f;
	mProjectileMovement->ProjectileGravityScale = 0.f;
	mProjectileMovement->bAutoActivate = false;
}

void AValkyrieProjectile::ProjectileStart()
{
	PrintViewport(4.f, FColor::Yellow, FString("AValkyrieProjectile::ProjectileStart()"));
	mProjectileMovement->Activate(true);
}

void AValkyrieProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
	mSphere->OnComponentBeginOverlap.AddDynamic(this, &AValkyrieProjectile::OnSphereOverlap);
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
