// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "EnhancedInputComponent.h"
#include "AGInputConfig.h"
#include "InputAction.h"
#include "AGInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class AG_API UAGInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityAction(const UAGInputConfig* InputConfig, UserClass* Object,
		PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
inline void UAGInputComponent::BindAbilityAction(const UAGInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);

	for (const UAGInputConfig& action : InputConfig->mAbilityInputActions)
	{
		if (action.mInputAction && action.mInputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(action.mInputAction, ETriggerEvent::Started, Obejct, PressedFunc, Action.mInputTag);
			}

			if (ReleasedFunc)
			{
				BindAction(action.mInputAction, ETriggerEvent::Completed, Obejct, ReleasedFunc, Action.mInputTag);
			}

			if (HeldFunc)
			{
				BindAction(action.mInputAction, ETriggerEvent::Triggered, Obejct, HeldFunc, Action.mInputTag);
			}
		}
	}
}
