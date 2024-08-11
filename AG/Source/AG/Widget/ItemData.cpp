// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemData.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "../AbilitySystem/AGAttributeSet.h"
#include "../AbilitySystem/AGAbilitySystemComponent.h"

UItemData::UItemData()
{
}

UItemData::~UItemData()
{
}

void UItemData::ApplyEffect(ACharacter* target)
{
	UAbilitySystemComponent* targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(target);
	if (targetASC == nullptr)
		return;
	UAGAbilitySystemComponent* asc = Cast<UAGAbilitySystemComponent>(targetASC);
	if (asc == nullptr)
		return;

	FGameplayEffectContextHandle ecHandle = asc->MakeEffectContext();
	ecHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle esHandle = asc->MakeOutgoingSpec(mEffect, 1.f, ecHandle);
	const FActiveGameplayEffectHandle agpeHandle = asc->ApplyGameplayEffectSpecToSelf(*esHandle.Data.Get());
}
