// Fill out your copyright notice in the Description page of Project Settings.


#include "ValkyrieLightning.h"
#include "../Collision/CollisionActor.h"

AValkyrieLightning::AValkyrieLightning()
{
}

void AValkyrieLightning::OnSystemFinish(UNiagaraComponent* PSystem)
{
	Super::OnSystemFinish(PSystem);

	mCollisionActor->ClearTimer();
	PSystem->DestroyComponent();
	Destroy();
}

void AValkyrieLightning::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters	SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	mCollisionActor = GetWorld()->SpawnActor<ACollisionActor>(
		GetActorLocation(),
		FRotator::ZeroRotator,
		SpawnParam
		);

	GetWorld()->GetTimerManager().SetTimer(mTimer, this, &AValkyrieLightning::SpawnCollisionActor, 0.3f, false);
}

void AValkyrieLightning::SpawnCollisionActor()
{
	mCollisionActor->SetHitType(EHitType::EHT_Continuous);

	mCollisionActor->SetActorLocation(GetActorLocation());
	mCollisionActor->SetCollisionShape(ECollisionType::ECS_Box);
	mCollisionActor->SetCollisionProfileName(FName("PlayerSword"));
	mCollisionActor->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	mCollisionActor->SetBoxExtent(FVector(50.f));
}
