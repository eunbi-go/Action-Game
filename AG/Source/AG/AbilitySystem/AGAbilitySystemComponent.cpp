// Fill out your copyright notice in the Description page of Project Settings.


#include "AGAbilitySystemComponent.h"

void UAGAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAGAbilitySystemComponent::EffectApplied);
}


void UAGAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	PrintViewport(1.f, FColor::Red, FString("EffectApplied"));
}
