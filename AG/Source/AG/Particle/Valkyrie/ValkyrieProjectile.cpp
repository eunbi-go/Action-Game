// Fill out your copyright notice in the Description page of Project Settings.


#include "ValkyrieProjectile.h"

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
	bReplicates = true;


	mSphere = CreateDefaultSubobject<USphereComponent>("SphereComp");
	mSphere->SetupAttachment(GetRootComponent());
	mSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	mSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	mSphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	mSphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	mSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	mProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile");
	mProjectileMovement->InitialSpeed = 550.f;
	mProjectileMovement->MaxSpeed = 550.f;
	mProjectileMovement->ProjectileGravityScale = 0.f;

}

void AValkyrieProjectile::BeginPlay()
{
	Super::BeginPlay();
	mSphere->OnComponentBeginOverlap.AddDynamic(this, &AValkyrieProjectile::OnSphereOverlap);
	
}

void AValkyrieProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AValkyrieProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}
