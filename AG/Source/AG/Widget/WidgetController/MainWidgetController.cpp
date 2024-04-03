// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidgetController.h"
#include "../../AbilitySystem/AGAttributeSet.h"
#include "../../AbilitySystem/AGAbilitySystemComponent.h"

void UMainWidgetController::BroadcastInitValues()
{
	const UAGAttributeSet* as = CastChecked<UAGAttributeSet>(mAttributeSet);
	mOnHpChange.Broadcast(as->GetmHp());
	mOnMaxHpChange.Broadcast(as->GetmMaxHp());
	mOnMpChange.Broadcast(as->GetmMp());
	mOnMaxMpChange.Broadcast(as->GetmMaxMp());
	mOnCoinChange.Broadcast(as->GetmCoin());
}

void UMainWidgetController::BindCallbacksToDependecies()
{
	const UAGAttributeSet* as = CastChecked<UAGAttributeSet>(mAttributeSet);

	mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
		as->GetmHpAttribute()).AddUObject(
			this, &UMainWidgetController::HpChange);
	mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
		as->GetmMaxHpAttribute()).AddUObject(
			this, &UMainWidgetController::MaxHpChange);

	mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
		as->GetmMpAttribute()).AddUObject(
			this, &UMainWidgetController::MpChange);
	mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
		as->GetmMaxMpAttribute()).AddUObject(
			this, &UMainWidgetController::MaxMpChange);

	mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
		as->GetmCoinAttribute()).AddUObject(
			this, &UMainWidgetController::CoinChange);

	Cast<UAGAbilitySystemComponent>(mAbilitySystemComp)->mEffectAssetTags.AddLambda(
		[](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& tag : AssetTags)
			{
				// ���� ��Ʈ�ѷ��� �±׸� ��ε�ĳ��Ʈ�Ѵ�. 
				// -> GameplayEffect�� ����Ǹ� �ش� Effect Blueprint�� �߰��� ��� Asset tag�� ���� �� �ִ�.
				const FString msg = FString::Printf(TEXT("GE Tag : %s"), *tag.ToString());
				PrintViewport(5.f, FColor::Red, msg);
			}
		}
	);
}

void UMainWidgetController::HpChange(const FOnAttributeChangeData& data) const
{
	// NewValue : ��� ����� ���ο� ���� ��´�.
	mOnHpChange.Broadcast(data.NewValue);
}

void UMainWidgetController::MaxHpChange(const FOnAttributeChangeData& data) const
{
	mOnMaxHpChange.Broadcast(data.NewValue);
}

void UMainWidgetController::MpChange(const FOnAttributeChangeData& data) const
{
	mOnMpChange.Broadcast(data.NewValue);
}

void UMainWidgetController::MaxMpChange(const FOnAttributeChangeData& data) const
{
	mOnMaxMpChange.Broadcast(data.NewValue);
}

void UMainWidgetController::CoinChange(const FOnAttributeChangeData& data) const
{
	mOnCoinChange.Broadcast(data.NewValue);
}
