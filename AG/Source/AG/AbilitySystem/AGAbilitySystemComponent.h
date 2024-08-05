// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "AbilitySystemComponent.h"
#include "AGAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&);

/**
 * 
 */
UCLASS()
class AG_API UAGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Actor Info�� ������ ��, ȣ��ȴ�. 
	void AbilityActorInfoSet();


	FEffectAssetTags mEffectAssetTags;

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	// Valkyrie �Է��� ���� ��, ȣ���Ѵ�.
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	


protected:
	// Reliable : ��Ŷ�� �սǵ� ��쿡�� Ŭ���̾�Ʈ�� �����Ѵٴ� ���� ����Ǹ� ������ Ȯ�ι��� ������ ���۵ȴ�. 
	UFUNCTION(Client, Reliable)
	// ASC�� ����Ǵ� ��� ȿ���� ȣ��ȴ�.
	void ClientEffectApplied(
		UAbilitySystemComponent* AbilitySystemComponent, 
		const FGameplayEffectSpec& EffectSpec, 
		FActiveGameplayEffectHandle ActiveEffectHandle
	);

};




