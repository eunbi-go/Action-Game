// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidgetController.h"
#include "../../AbilitySystem/AGAttributeSet.h"
#include "../../AbilitySystem/AGAbilitySystemComponent.h"
#include "../../AGGameInstance.h"
#include "../../Player/ValkyriePlayerState.h"

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
		as->GetmHpAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				mOnHpChange.Broadcast(Data.NewValue);
			});
	mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
		as->GetmMaxHpAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				mOnMaxHpChange.Broadcast(Data.NewValue);
			});


	mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
		as->GetmMpAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				mOnMpChange.Broadcast(Data.NewValue);
			});
	mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
		as->GetmMaxMpAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				mOnMaxMpChange.Broadcast(Data.NewValue);
			});

	mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
		as->GetmCoinAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				mOnCoinChange.Broadcast(Data.NewValue);
			});



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