// Fill out your copyright notice in the Description page of Project Settings.


#include "AGAbilitySystemComponent.h"
#include "../AGGameplayTags.h"
#include "Ability/AGGameplayAbility.h"

void UAGAbilitySystemComponent::AbilityActorInfoSet()
{
	/*
	* ���ο��� ����Ǵ� ��������Ʈ
	* ASC�� ����Ǵ� ��� ȿ���� ���� �������� ȣ��Ǵ� �ݹ�(EffectApplied)�� ������.
	*/ 
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAGAbilitySystemComponent::EffectApplied);
	const FAGGameplayTags& gameplayTags = FAGGameplayTags::Get();
}


void UAGAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec abilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UAGGameplayAbility* ability = Cast<UAGGameplayAbility>(abilitySpec.Ability))
		{
			abilitySpec.DynamicAbilityTags.AddTag(ability->mStartupInputTag);
			// GiveAbility() : Ability �߰�, Ȱ��ȭX
			GiveAbility(abilitySpec);
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
				// Ability�� Ȱ��ȭ��Ų��.
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
			CancelAbility(abilitySpec.Ability);
		}
	}
}

void UAGAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	/*
	 
	*/
	// �迭�� �±׸� �������� �ʰ�, �����÷����±� �����̳ʿ� �����Ѵ�.
	FGameplayTagContainer tagContainer;
	EffectSpec.GetAllAssetTags(tagContainer);


	// tagContainer�� �ִ� ��� �±׿� ���� ���� ��Ʈ�ѷ����� ��ε�ĳ��Ʈ�Ѵ�.
	// �̷��� �ϸ� GameplayEffect�� �±׸� �߰��� �� �ش� �±׸� ������ ��ε�ĳ��Ʈ�ϰ�,
	// ȭ�鿡 ������ �޽����� ǥ���ϴµ� ����� �� �ִ�.
	mEffectAssetTags.Broadcast(tagContainer);

	


	PrintViewport(1.f, FColor::Red, FString("UAGAbilitySystemComponent::EffectApplied"));
}