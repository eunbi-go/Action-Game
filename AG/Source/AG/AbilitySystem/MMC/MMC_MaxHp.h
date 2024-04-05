// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHp.generated.h"

/**
 * Attribute 뿐만 아니라 Attribute가 아닌 
 * 다른 변수에도 의존할 수 있도록
 * 설계한 사용자 정의 계산 클래스
 */
UCLASS()
class AG_API UMMC_MaxHp : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_MaxHp();

	// Spec : 계산에 사용할 GameplayEffect 의 Spec
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;


private:
	// 캡처할 Attribute
	FGameplayEffectAttributeCaptureDefinition mResilienceDef;
};
