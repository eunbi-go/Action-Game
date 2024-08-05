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
	// Actor Info를 설정한 후, 호출된다. 
	void AbilityActorInfoSet();


	FEffectAssetTags mEffectAssetTags;

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	// Valkyrie 입력이 있을 때, 호출한다.
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	


protected:
	// Reliable : 패킷이 손실된 경우에도 클라이언트에 도달한다는 것이 보장되며 수신을 확인받을 때까지 전송된다. 
	UFUNCTION(Client, Reliable)
	// ASC에 적용되는 모든 효과에 호출된다.
	void ClientEffectApplied(
		UAbilitySystemComponent* AbilitySystemComponent, 
		const FGameplayEffectSpec& EffectSpec, 
		FActiveGameplayEffectHandle ActiveEffectHandle
	);

};




