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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return mAttributeSet; }

	FORCEINLINE int32 GetPlayerLevel() const
	{
		return mLevel;
	}

protected:
	UPROPERTY(VisibleAnywhere)
	UAbilitySystemComponent* mAbilitySystemComp;

	UPROPERTY(VisibleAnywhere)
	UAttributeSet* mAttributeSet;

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32	mLevel = 1;

	UFUNCTION()
	void OnRep_Level(int32 PreLevel);
};




