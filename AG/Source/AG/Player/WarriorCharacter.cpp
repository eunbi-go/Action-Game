// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorCharacter.h"

AWarriorCharacter::AWarriorCharacter()
{
	//------------------
	// Mesh ¼¼ÆÃ.
	//------------------
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>    characterAsset(TEXT("SkeletalMesh'/Game/Assets/Character/Valkyrie/Mesh/SK_Valkyrie.SK_Valkyrie'"));

	if (characterAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(characterAsset.Object);
	}

	GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -90.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));
}
