// Fill out your copyright notice in the Description page of Project Settings.


#include "AGGameplayTags.h"
#include "GameplayTagsManager.h"

FAGGameplayTags FAGGameplayTags::mGameplayTags;

void FAGGameplayTags::InitializeNativeGameplayTags()
{

	mGameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"),
		FString("Increase physical damage")
	);
	mGameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"),
		FString("Increase megical damage")
	);
	mGameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Resilience"),
		FString("Increase Armor")
	);
	mGameplayTags.Attributes_Primary_Dexterity = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Dexterity"),
		FString("Increase Block Percent")
	);
	mGameplayTags.Attributes_Primary_Coin = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Coin"),
		FString("Coin")
	);


	mGameplayTags.Attributes_Second_MaxHp = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Second.MaxHp"), 
		FString("max hp")
	);
	mGameplayTags.Attributes_Second_MaxMp = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Second.MaxMp"),
		FString("max mp")
	);
	mGameplayTags.Attributes_Second_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Second.Armor"),
		FString("Reduces damage")
	);
	mGameplayTags.Attributes_Second_BlockPercent = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Second.BlockPercent"),
		FString("Percent to reduce damage")
	);
	mGameplayTags.Attributes_Second_CriticalPercent = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Second.CriticalPercent"),
		FString("Percent to double damage  plus hit")
	);
	mGameplayTags.Attributes_Second_CriticalDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Second.CriticalDamage"),
		FString("Bonus damage when critical scored")
	);
	mGameplayTags.Attributes_Second_CriticalResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Second.CriticalResistance"),
		FString("Reduce Critical hit damage")
	);






	mGameplayTags.InputTag_FWD = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.FWD"),
		FString("Input Tag for Forward")
	);
	mGameplayTags.InputTag_BWD = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.BWD"),
		FString("Input Tag for Backward")
	);
	mGameplayTags.InputTag_RT = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RT"),
		FString("Input Tag for Right")
	);
	mGameplayTags.InputTag_LT = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LT"),
		FString("Input Tag for Left")
	);
	mGameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.1"),
		FString("Input Tag for 1 key")
	);
	mGameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.2"),
		FString("Input Tag for 2 key")
	);
	mGameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.3"),
		FString("Input Tag for 3 key")
	);
	mGameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.4"),
		FString("Input Tag for 4 key")
	);
}
