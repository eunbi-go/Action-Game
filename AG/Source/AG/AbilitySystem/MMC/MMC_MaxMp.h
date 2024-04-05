// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxMp.generated.h"

/**
 * 
 */
UCLASS()
class AG_API UMMC_MaxMp : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UMMC_MaxMp();

	// Spec : ��꿡 ����� GameplayEffect �� Spec
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;


private:
	// ĸó�� Attribute
	FGameplayEffectAttributeCaptureDefinition mIntelligenceDef;

};
