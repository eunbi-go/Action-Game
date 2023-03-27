// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorCharacter.h"
#include "../Particle/ParticleCascade.h"

AWarriorCharacter::AWarriorCharacter()
{
	//------------------
	// Mesh ����.
	//------------------
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>    characterAsset(TEXT("SkeletalMesh'/Game/Assets/Character/Valkyrie/Mesh/SK_Valkyrie.SK_Valkyrie'"));

	if (characterAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(characterAsset.Object);

	GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -90.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));




	//-----------------------------------------
	// AnimInstance ����.
	//-----------------------------------------
	static ConstructorHelpers::FClassFinder<UAnimInstance>	animInst(TEXT("AnimBlueprint'/Game/Blueprints/Player/Animation/ABP_Warrior.ABP_Warrior_C'"));
	
	if (animInst.Succeeded())
		GetMesh()->SetAnimInstanceClass(animInst.Class);



	//-----------------------------------------
	// Particle ����.
	//-----------------------------------------
	mDashParticle = CreateDefaultSubobject<AParticleCascade>(TEXT("DashParticle"));

	AParticleCascade* particle = Cast<AParticleCascade>(mDashParticle);
	particle->SetParticle(TEXT("ParticleSystem'/Game/Assets/InfinityBladeEffects1/Effects/FX_Ambient/Fire/P_LavaDrips.P_LavaDrips'"));
	//particle->SetParticle(TEXT("ParticleSystem'/Game/Assets/InfinityBladeEffects1/Effects/FX_Ambient/Fire/P_Fire_TrapBossEnd_mobile.P_Fire_TrapBossEnd_mobile'"));
	//Particle->SetSound(TEXT("SoundWave'/Game/Sound/Fire1.Fire1'"), false);
}
