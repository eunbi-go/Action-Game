// Fill out your copyright notice in the Description page of Project Settings.


#include "ValkyriePlayerState.h"
#include "../AbilitySystem/AGAbilitySystemComponent.h"
#include "../AbilitySystem/AGAttributeSet.h"


AValkyriePlayerState::AValkyriePlayerState()
{
	mAbilitySystemComp = CreateDefaultSubobject<UAGAbilitySystemComponent>("AbilitySystemComp");
	mAbilitySystemComp->SetIsReplicated(true);
	mAbilitySystemComp->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	mAttributeSet = CreateDefaultSubobject<UAGAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* AValkyriePlayerState::GetAbilitySystemComponent() const
{
	return mAbilitySystemComp;
}
