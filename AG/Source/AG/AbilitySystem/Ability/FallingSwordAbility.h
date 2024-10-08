// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "../Ability/AGGameplayAbility.h"
#include "FallingSwordAbility.generated.h"

/**
 * 
 */
UCLASS()
class AG_API UFallingSwordAbility : public UAGGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "FallingSword")
	void SpawnEffect();

	UFUNCTION(BlueprintCallable, Category = "FallingSword")
	void StartCameraShake();

	UFUNCTION(BlueprintCallable, Category = "FallingSword")
	void AbilityEnd();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AParticleNiagara> mEffectClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> mDamageEffectClass;

	UPROPERTY()
	FTimerHandle	mTimer;
};
