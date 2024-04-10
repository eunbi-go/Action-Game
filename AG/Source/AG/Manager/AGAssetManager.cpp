// Fill out your copyright notice in the Description page of Project Settings.


#include "AGAssetManager.h"
#include "../AGGameplayTags.h"

UAGAssetManager& UAGAssetManager::Get()
{
	UAGAssetManager* assetManager = Cast<UAGAssetManager>(GEngine->AssetManager);

	return *assetManager;
}

void UAGAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FAGGameplayTags::InitializeNativeGameplayTags();
}
