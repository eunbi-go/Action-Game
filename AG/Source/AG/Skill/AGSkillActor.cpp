// Fill out your copyright notice in the Description page of Project Settings.


#include "AGSkillActor.h"

AAGSkillActor::AAGSkillActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AAGSkillActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAGSkillActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAGSkillActor::SpawnEffect()
{
}

void AAGSkillActor::FindTarget()
{
}

void AAGSkillActor::SkillEnd()
{
}

void AAGSkillActor::SetOwnerActor(AActor* OwnerActor)
{
	mOwner = OwnerActor;
}

