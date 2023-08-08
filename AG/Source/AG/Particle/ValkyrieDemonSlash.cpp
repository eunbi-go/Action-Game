// Fill out your copyright notice in the Description page of Project Settings.


#include "ValkyrieDemonSlash.h"
#include "../Collision/CollisionActor.h"

AValkyrieDemonSlash::AValkyrieDemonSlash()
{
}

void AValkyrieDemonSlash::BeginPlay()
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
	mCollisionActor->SetCollisionShape(ECollisionType::ECS_Capsule);
	mCollisionActor->SetCollisionProfileName(FName("PlayerSword"));
	mCollisionActor->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	mCollisionActor->SetSphereRadius(210.f);
	mCollisionActor->SetHitType(EHitType::EHT_Once);
}


void AValkyrieDemonSlash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AValkyrieDemonSlash::OnSystemFinish(UNiagaraComponent* PSystem)
{
	mCollisionActor->Destroy();
	Destroy();
}

