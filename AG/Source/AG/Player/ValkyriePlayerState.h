// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "ValkyriePlayerState.generated.h"

/**
 * 
 */

class UAbilitySystemComponent;
class UAttributeSet;
UCLASS()
class AG_API AValkyriePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AValkyriePlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return mAttributeSet; }

protected:
	UPROPERTY()
		TObjectPtr<UAbilitySystemComponent> mAbilitySystemComp;

	UPROPERTY()
		TObjectPtr<UAttributeSet> mAttributeSet;
};
