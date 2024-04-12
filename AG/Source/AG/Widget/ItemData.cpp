// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemData.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "../AbilitySystem/AGAttributeSet.h"

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

	FGameplayEffectContextHandle ecHandle = targetASC->MakeEffectContext();
	ecHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle esHandle = targetASC->MakeOutgoingSpec(mEffect, 1.f, ecHandle);
	const FActiveGameplayEffectHandle agpeHandle = targetASC->ApplyGameplayEffectSpecToSelf(*esHandle.Data.Get());}
