// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BasicInfo.h"
#include "GameplayTagContainer.h"

/**
 * 
 */


struct FAGGameplayTags
{
public:
	static const FAGGameplayTags& Get() { return mGameplayTags; }
	static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Dexterity;
	FGameplayTag Attributes_Primary_Coin;

	FGameplayTag Attributes_Second_MaxHp;
	FGameplayTag Attributes_Second_MaxMp;
	FGameplayTag Attributes_Second_Armor;
	FGameplayTag Attributes_Second_BlockPercent;
	FGameplayTag Attributes_Second_CriticalPercent;
	FGameplayTag Attributes_Second_CriticalDamage;
	FGameplayTag Attributes_Second_CriticalResistance;


	FGameplayTag InputTag_FWD;
	FGameplayTag InputTag_BWD;
	FGameplayTag InputTag_RT;
	FGameplayTag InputTag_LT;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;

protected:
private:
	static FAGGameplayTags mGameplayTags;
};