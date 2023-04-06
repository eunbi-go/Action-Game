// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterPatrolPoint.h"

AMonsterPatrolPoint::AMonsterPatrolPoint()
{
	PrimaryActorTick.bCanEverTick = true;

	mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	SetRootComponent(mRoot);

	mRoot->bVisualizeComponent = true;
}

void AMonsterPatrolPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMonsterPatrolPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

