// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "../Ability/AGGameplayAbility.h"
#include "SprintAbility.generated.h"

/**
 * 
 */
UCLASS()
class AG_API USprintAbility : public UAGGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void SpawnEffect(const FVector& TargetLocation);

	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void SetTarget(const FVector& TargetLocation);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AParticleNiagara> mEffectClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> mDamageEffectClass;
};
