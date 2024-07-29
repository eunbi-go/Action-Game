// Fill out your copyright notice in the Description page of Project Settings.


#include "AGInputConfig.h"
#include "InputAction.h"

const UInputAction* UAGInputConfig::FindAbilityInputActionByTag(const FGameplayTag& InputTag)
{
	for (const FAGInputAction& action : mAbilityInputActions)
	{
		if (action.mInputAction && action.mInputTag == InputTag)
			return action.mInputAction;
	}

	return nullptr;
}
