// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "Engine/AssetManager.h"
#include "AGAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class AG_API UAGAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	static UAGAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
