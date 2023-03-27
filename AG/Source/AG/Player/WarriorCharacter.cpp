// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorCharacter.h"

#include "../Particle/ParticleCascade.h"
#include "../Basic/WeaponActor.h"

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
	//mDashParticle = CreateDefaultSubobject<AParticleCascade>(TEXT("DashParticle"));

	//AParticleCascade* particle = Cast<AParticleCascade>(mDashParticle);
	//particle->SetParticle(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Combat_Base/WeaponShimmer/Fire/P_WeaponShimmer_Fire_Blade.P_WeaponShimmer_Fire_Blade'"));
	//particle->SetParticle(TEXT("ParticleSystem'/Game/Assets/InfinityBladeEffects1/Effects/FX_Ambient/Fire/P_Fire_TrapBossEnd_mobile.P_Fire_TrapBossEnd_mobile'"));
	//Particle->SetSound(TEXT("SoundWave'/Game/Sound/Fire1.Fire1'"), false);
}

void AWarriorCharacter::BeginPlay()
{
	Super::BeginPlay();

	//-----------------------------------------
	// ���� ����.
	//-----------------------------------------
	FActorSpawnParameters	SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	mWeapon = GetWorld()->SpawnActor<AWeaponActor>(
		AWeaponActor::StaticClass(), SpawnParam);

	mWeapon->SetSkeletalMesh(TEXT("StaticMesh'/Game/Assets/Weapons/Sword1.Sword1'"));

	mWeapon->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::KeepRelativeTransform,
		TEXT("thumb_02_r"));

}
