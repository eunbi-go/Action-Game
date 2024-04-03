// Fill out your copyright notice in the Description page of Project Settings.


#include "AGAbilitySystemComponent.h"

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
