// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeWidgetController.h"
#include "../../AbilitySystem/AGAttributeSet.h"
#include "../../AbilitySystem/Data/AttributeInfo.h"
#include "../../AGGameplayTags.h"

UAttributeWidgetController::UAttributeWidgetController()
{
	static ConstructorHelpers::FObjectFinder<UAttributeInfo> curve(
		TEXT("AttributeInfo'/Game/Blueprints/AbilitySystem/Data/DA_AttributeInfo2.DA_AttributeInfo2'")
	);
	if (curve.Succeeded())
	{
		mAttributeInfo = curve.Object;
	}

	int32 k = 0;
}

void UAttributeWidgetController::BroadcastInitValues()
{
	UAGAttributeSet* as = CastChecked<UAGAttributeSet>(mAttributeSet);
	check(mAttributeInfo);

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

	for (auto& pair : as->mTagsToAttributes)
	{
		// pair.Value.Execute() : as->GetmStrengthAttribute()
		mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
			pair.Value.Execute()).AddLambda(
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
