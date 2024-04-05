// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHp.generated.h"

/**
 * Attribute �Ӹ� �ƴ϶� Attribute�� �ƴ� 
 * �ٸ� �������� ������ �� �ֵ���
 * ������ ����� ���� ��� Ŭ����
 */
UCLASS()
class AG_API UMMC_MaxHp : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_MaxHp();

	// Spec : ��꿡 ����� GameplayEffect �� Spec
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;


private:
	// ĸó�� Attribute
	FGameplayEffectAttributeCaptureDefinition mResilienceDef;
};
