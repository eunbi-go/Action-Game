// Fill out your copyright notice in the Description page of Project Settings.


#include "ValkyrieBlinkFire.h"
#include "../Collision/CollisionActor.h"

AValkyrieBlinkFire::AValkyrieBlinkFire()
{
}

void AValkyrieBlinkFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AValkyrieBlinkFire::OnSystemFinish(UNiagaraComponent* PSystem)
{
	mCollisionActor->Destroy();
	Destroy();
}

void AValkyrieBlinkFire::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters	SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	mCollisionActor = GetWorld()->SpawnActor<ACollisionActor>(
		GetActorLocation(),
		GetActorRotation(),
		SpawnParam
		);
	mCollisionActor->SetCollisionShape(ECollisionType::ECS_Sphere);
	mCollisionActor->SetCollisionProfileName(FName("PlayerSword"));
	mCollisionActor->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	mCollisionActor->SetSphereRadius(210.f);
	mCollisionActor->SetHitType(EHitType::EHT_Once);
	mCollisionActor->SetParent(this);
}
