// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "Engine/DataAsset.h"
#include "CharacterInfo.generated.h"

// 캐릭터 클래스들을 구분하기 위한 enum class
UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Warrior,
	Enemy_Sword,
	Enemy_Gun
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Class Default")
	TSubclassOf<UGameplayEffect> mPrimaryAttribute;
};

/**
 * 
 */
UCLASS()
class AG_API UCharacterInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Default")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> mCharacterInformation;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Default")
	TSubclassOf<UGameplayEffect> mSecondaryAttribute;

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Default")
	TSubclassOf<UGameplayEffect> mVitalAttribute;
};
