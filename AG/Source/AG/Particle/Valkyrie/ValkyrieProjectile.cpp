// Fill out your copyright notice in the Description page of Project Settings.


#include "ValkyrieProjectile.h"

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
