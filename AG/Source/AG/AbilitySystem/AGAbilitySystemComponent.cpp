// Fill out your copyright notice in the Description page of Project Settings.


#include "AGAbilitySystemComponent.h"

void UAGAbilitySystemComponent::AbilityActorInfoSet()
{
	/*
	* OnGameplayEffectAppliedDelegateToSelf
	* Called on server whenever a GE is applied to self.
	* This includes instant and duration based GEs.
	* ���ο��� ����Ǵ� ��������Ʈ
	* ASC�� ����Ǵ� ��� ȿ���� ���� �������� ȣ��Ǵ� �ݹ�(EffectApplied)�� ������.
	*/ 
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAGAbilitySystemComponent::EffectApplied);
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
