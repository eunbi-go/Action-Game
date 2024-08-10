// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "NearestTargetData.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNearestTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);

/**
 * 
 */
UCLASS()
class AG_API UNearestTargetData : public UAbilityTask
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks",
		meta = (DisplayName = "NearestTargetData",
			HidePin = "OwningAbility",
			DefaultToSelf = "OwningAbility", // 디폴트값 : self
			BlueprintInternalUseOnly = "true"))
	static UNearestTargetData* CreateNearestTargetData(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FNearestTargetDataSignature mValidData;

private:
	virtual void Activate() override;
	void SendTargetData();
	UFUNCTION()
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};
