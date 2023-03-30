// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportSkill.h"

ATeleportSkill::ATeleportSkill()
{
}

void ATeleportSkill::BeginPlay()
{
	Super::BeginPlay();
}

void ATeleportSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATeleportSkill::CollisionProjectile(const FHitResult& Hit)
{
	//mOnSkillEnd.Broadcast(this, Hit);
}
