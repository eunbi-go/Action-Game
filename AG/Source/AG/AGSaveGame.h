// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BasicInfo.h"
#include "GameFramework/SaveGame.h"
#include "AGSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class AG_API UAGSaveGame : public USaveGame
{
	GENERATED_BODY()
	
	friend class APlayerCharacter;
	friend class AAGGameModeBase;
	friend class UCharacterStatComponent;


private:
	UPROPERTY()
	FPlayerInfo	mPlayerInfo;

	UPROPERTY()
	FVector	mLocation;

	UPROPERTY()
	FVector	mScale;

	UPROPERTY()
	FRotator	mRotation;
};
