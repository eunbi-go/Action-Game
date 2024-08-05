// Fill out your copyright notice in the Description page of Project Settings.


#include "AGAssetManager.h"
#include "../AGGameplayTags.h"
#include "AbilitySystemGlobals.h"

UAGAssetManager& UAGAssetManager::Get()
{
	UAGAssetManager* assetManager = Cast<UAGAssetManager>(GEngine->AssetManager);

	return *assetManager;
}

void UAGAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FAGGameplayTags::InitializeNativeGameplayTags();
	
	// Target Data를 받을 때 필요하다
	UAbilitySystemGlobals::Get().InitGlobalData();
}
