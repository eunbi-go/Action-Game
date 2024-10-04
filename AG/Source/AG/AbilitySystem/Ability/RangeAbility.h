// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "../Ability/AGGameplayAbility.h"
#include "RangeAbility.generated.h"

/**
 * 
 */
UCLASS()
class AG_API URangeAbility : public UAGGameplayAbility
{
	GENERATED_BODY()


protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	UFUNCTION()
	void SpawnEffect(const float& PressingTime);

	UFUNCTION(BlueprintCallable, Category = "Range")
	void Pause();

	UFUNCTION(BlueprintCallable, Category = "Range")
	void AbilityEnd();
	
	UFUNCTION(BlueprintCallable, Category = "Range")
	bool CheckStartEnable();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class ARangeEffect> mEffectClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> mDamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UValkyrieAnimInstance> mAnimInst;

	bool mIsSpawnEffect = false;
};
