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

	// ��Ÿ�ӿ� �Է� �±װ� ����� �� �����Ƿ� ���� �Է� �±׿��� ���ȴ�.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	FGameplayTag mStartupInputTag;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayEffect>> mEffects;
};
