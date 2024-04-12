// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeWidgetController.h"
#include "../../AbilitySystem/AGAttributeSet.h"
#include "../../AbilitySystem/Data/AttributeInfo.h"
#include "../../AGGameplayTags.h"

UAttributeWidgetController::UAttributeWidgetController()
{
	static ConstructorHelpers::FObjectFinder<UAttributeInfo> curve(TEXT("AttributeInfo'/Game/Blueprints/AbilitySystem/Data/DA_AttirbuteInfo.DA_AttirbuteInfo'"));
	if (curve.Succeeded())
	{
		mAttributeInfo = curve.Object;
	}
}

void UAttributeWidgetController::BroadcastInitValues()
{
	UAGAttributeSet* as = CastChecked<UAGAttributeSet>(mAttributeSet);
	check(mAttributeInfo);

	//FAGAttributeInfo info = mAttributeInfo->FindAttributeInfoByTag(FAGGameplayTags::Get().Attributes_Primary_Strength);
	//info.mAttributeValue = as->GetmStrength();
	//mAttributeInfoDelegate.Broadcast(info);

	for (auto& pair : as->mTagsToAttributes)
	{
		FAGAttributeInfo info = mAttributeInfo->FindAttributeInfoByTag(pair.Key);
		info.mAttributeValue = pair.Value.Execute().GetNumericValue(as);
		info.mAttributeTag = pair.Key;
		mAttributeInfoDelegate.Broadcast(info);
	}
}

void UAttributeWidgetController::BindCallbacksToDependecies()
{
	UAGAttributeSet* as = CastChecked<UAGAttributeSet>(mAttributeSet);
	const FAGGameplayTags& gameplayTags = FAGGameplayTags::Get();
	

	for (auto& pair : as->mTagsToAttributes)
	{
		mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
			as->GetmStrengthAttribute()).AddLambda(
				[this, pair, as](const FOnAttributeChangeData& Data)
				{
					FAGAttributeInfo info = mAttributeInfo->FindAttributeInfoByTag(pair.Key);
					info.mAttributeValue = pair.Value.Execute().GetNumericValue(as);
					info.mAttributeTag = pair.Key;
					mAttributeInfoDelegate.Broadcast(info);
				}
		);

		mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
			as->GetmDexterityAttribute()).AddLambda(
				[this, pair, as](const FOnAttributeChangeData& Data)
				{
					FAGAttributeInfo info = mAttributeInfo->FindAttributeInfoByTag(pair.Key);
					info.mAttributeValue = pair.Value.Execute().GetNumericValue(as);
					info.mAttributeTag = pair.Key;
					mAttributeInfoDelegate.Broadcast(info);
				}
		);

		mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
			as->GetmBlockPercentAttribute()).AddLambda(
				[this, pair, as](const FOnAttributeChangeData& Data)
				{
					FAGAttributeInfo info = mAttributeInfo->FindAttributeInfoByTag(pair.Key);
					info.mAttributeValue = pair.Value.Execute().GetNumericValue(as);
					info.mAttributeTag = pair.Key;
					mAttributeInfoDelegate.Broadcast(info);
				}
		);

	}


}
