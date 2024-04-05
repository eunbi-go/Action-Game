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

	// Spec : 계산에 사용할 GameplayEffect 의 Spec
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;


private:
	// 캡처할 Attribute
	FGameplayEffectAttributeCaptureDefinition mIntelligenceDef;

};
