// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionObject.h"

ACollisionObject::ACollisionObject()
{
	PrimaryActorTick.bCanEverTick = true;
	mCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	mCollisionBox->BodyInstance.SetCollisionProfileName(TEXT("MonsterAttack"));
	mCollisionBox->SetNotifyRigidBodyCollision(true);
	mCollisionBox->SetBoxExtent(FVector(100.f));
	mCollisionBox->SetSimulatePhysics(true);
}

void ACollisionObject::BeginPlay()
{
	Super::BeginPlay();
	mCollisionBox->OnComponentHit.AddDynamic(this, &ACollisionObject::OnHit);
}

void ACollisionObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACollisionObject::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	PrintViewport(1.f, FColor::Green, TEXT("ACollisionObject::OnHit"));
	mOnHitt.Broadcast(this, Hit, OtherActor);
}

