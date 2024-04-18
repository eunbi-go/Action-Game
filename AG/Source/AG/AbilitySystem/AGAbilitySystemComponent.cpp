// Fill out your copyright notice in the Description page of Project Settings.


#include "AGAbilitySystemComponent.h"
#include "../AGGameplayTags.h"
#include "Ability/AGGameplayAbility.h"

void UAGAbilitySystemComponent::AbilityActorInfoSet()
{
	/*
	* OnGameplayEffectAppliedDelegateToSelf
	* Called on server whenever a GE is applied to self.
	* This includes instant and duration based GEs.
	* 본인에게 적용되는 델리게이트
	* ASC에 적용되는 모든 효과에 대한 응답으로 호출되는 콜백(EffectApplied)을 적용함.
	*/ 
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAGAbilitySystemComponent::EffectApplied);


	const FAGGameplayTags& gameplayTags = FAGGameplayTags::Get();
}


void UAGAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec abilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UAGGameplayAbility* ability = Cast<UAGGameplayAbility>(abilitySpec.Ability))
		{
			abilitySpec.DynamicAbilityTags.AddTag(ability->mStartupInputTag);
			// GiveAbility() : Ability 추가, 활성화X
			GiveAbility(abilitySpec);
			//// GiveAbilityAndActivateOnce() : Ability 추가, 활성화 O
			//GiveAbilityAndActivateOnce(abilitySpec);
		}
		
	}
}

void UAGAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;


	for (FGameplayAbilitySpec& abilitySpec : GetActivatableAbilities())
	{
		if (abilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(abilitySpec);
			if (!abilitySpec.IsActive())
			{
				TryActivateAbility(abilitySpec.Handle);
			}
		}
	}
}

void UAGAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;


	for (FGameplayAbilitySpec& abilitySpec : GetActivatableAbilities())
	{
		if (abilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(abilitySpec);
		}
	}
}

void UAGAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	/*
	 
	*/
	// 배열에 태그를 저장하지 않고, 게임플레이태그 컨테이너에 저장한다.
	FGameplayTagContainer tagContainer;
	EffectSpec.GetAllAssetTags(tagContainer);


	// tagContainer에 있는 모든 태그에 대해 위젯 컨트롤러에게 브로드캐스트한다.
	// 이렇게 하면 GameplayEffect에 태그를 추가할 때 해당 태그를 가져와 브로드캐스트하고,
	// 화면에 일종의 메시지를 표시하는데 사용할 수 있다.
	mEffectAssetTags.Broadcast(tagContainer);

	


	PrintViewport(1.f, FColor::Red, FString("UAGAbilitySystemComponent::EffectApplied"));
}
