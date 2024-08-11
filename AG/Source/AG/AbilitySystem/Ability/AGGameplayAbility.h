// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "Abilities/GameplayAbility.h"
#include "AGGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AG_API UAGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// 런타임에 입력 태그가 변경될 수 있으므로 시작 입력 태그에만 사용된다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	FGameplayTag mStartupInputTag;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayEffect>> mEffects;
};
