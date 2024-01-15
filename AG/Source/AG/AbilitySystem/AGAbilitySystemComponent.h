// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "AbilitySystemComponent.h"
#include "AGAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class AG_API UAGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();
	
protected:
	// ASC�� ����Ǵ� ��� ȿ���� ȣ��ȴ�.
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};
