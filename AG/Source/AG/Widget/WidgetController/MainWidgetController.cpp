// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidgetController.h"
#include "../../AbilitySystem/AGAttributeSet.h"
#include "../../AbilitySystem/AGAbilitySystemComponent.h"
#include "../../AGGameInstance.h"

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


	UAGGameInstance* gameInst = Cast<UAGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));


	Cast<UAGAbilitySystemComponent>(mAbilitySystemComp)->mEffectAssetTags.AddLambda(
		[gameInst, this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& tag : AssetTags)
			{
				FGameplayTag messageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (tag.MatchesTag(messageTag))
				{
					// 위젯 테이블에서 적용된 태그를 검색하고 싶다.
					const FUIWidgetRow* row = gameInst->GetDataTableRowByTag<FUIWidgetRow>(tag);
					mOnMessageWidgetRowChange.Broadcast(*row);
				}
			}
		}
	);
}

void UMainWidgetController::HpChange(const FOnAttributeChangeData& data) const
{
	// NewValue : 방금 변경된 새로운 값을 얻는다.
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
