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





	// Input
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
	mGameplayTags.InputTag_NormalAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.NormalAttack"),
		FString("Input Tag for NormalAttack key")
	);

	mGameplayTags.InputTag_W = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.W"),
		FString("Input Tag for W key")
	);
	mGameplayTags.InputTag_A = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.A"),
		FString("Input Tag for A key")
	);
	mGameplayTags.InputTag_S = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.S"),
		FString("Input Tag for S key")
	);
	mGameplayTags.InputTag_D = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.D"),
		FString("Input Tag for D key")
	);

	mGameplayTags.InputTag_Q = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Q"),
		FString("Input Tag for Q key")
	);
	mGameplayTags.InputTag_E = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.E"),
		FString("Input Tag for E key")
	);
	mGameplayTags.InputTag_R = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.R"),
		FString("Input Tag for R key")
	);
	mGameplayTags.InputTag_T = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.T"),
		FString("Input Tag for T key")
	);
}
