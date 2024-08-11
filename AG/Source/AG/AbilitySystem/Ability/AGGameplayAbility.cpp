// Fill out your copyright notice in the Description page of Project Settings.


#include "AGGameplayAbility.h"
#include "../../CharacterBase/AGBaseCharacter.h"
#include "../AGAbilitySystemComponent.h"

void UAGGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
