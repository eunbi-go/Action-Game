// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeWidgetController.h"
#include "../../AbilitySystem/AGAttributeSet.h"
#include "../../AbilitySystem/Data/AttributeInfo.h"
#include "../../AGGameplayTags.h"

UAttributeWidgetController::UAttributeWidgetController()
{
	static ConstructorHelpers::FObjectFinder<UAttributeInfo> curve(TEXT("AttributeInfo'/Game/Blueprints/AbilitySystem/Data/DA_AttributeInfo2.DA_AttributeInfo2'"));
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



	//mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
	//	as->GetmStrengthAttribute()).AddLambda(
	//		[this, gameplayTags, as](const FOnAttributeChangeData& Data)
	//		{
	//			FAGAttributeInfo info = mAttributeInfo->FindAttributeInfoByTag(gameplayTags.Attributes_Primary_Strength);
	//			info.mAttributeValue = as->GetmStrength();
	//			info.mAttributeTag = gameplayTags.Attributes_Primary_Strength;
	//			mAttributeInfoDelegate.Broadcast(info);
	//		}
	//);

	//mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
	//	as->GetmDexterityAttribute()).AddLambda(
	//		[this, gameplayTags, as](const FOnAttributeChangeData& Data)
	//		{
	//			FAGAttributeInfo info = mAttributeInfo->FindAttributeInfoByTag(gameplayTags.Attributes_Primary_Dexterity);
	//			info.mAttributeValue = as->GetmDexterity();
	//			info.mAttributeTag = gameplayTags.Attributes_Primary_Dexterity;
	//			mAttributeInfoDelegate.Broadcast(info);
	//		}
	//);

	//mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
	//	as->GetmIntelligenceAttribute()).AddLambda(
	//		[this, gameplayTags, as](const FOnAttributeChangeData& Data)
	//		{
	//			FAGAttributeInfo info = mAttributeInfo->FindAttributeInfoByTag(gameplayTags.Attributes_Primary_Intelligence);
	//			info.mAttributeValue = as->GetmIntelligence();
	//			info.mAttributeTag = gameplayTags.Attributes_Primary_Intelligence;
	//			mAttributeInfoDelegate.Broadcast(info);
	//		}
	//);

	//mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
	//	as->GetmResilienceAttribute()).AddLambda(
	//		[this, gameplayTags, as](const FOnAttributeChangeData& Data)
	//		{
	//			FAGAttributeInfo info = mAttributeInfo->FindAttributeInfoByTag(gameplayTags.Attributes_Primary_Resilience);
	//			info.mAttributeValue = as->GetmResilience();
	//			info.mAttributeTag = gameplayTags.Attributes_Primary_Resilience;
	//			mAttributeInfoDelegate.Broadcast(info);
	//		}
	//);

	//mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
	//	as->GetmCoinAttribute()).AddLambda(
	//		[this, gameplayTags, as](const FOnAttributeChangeData& Data)
	//		{
	//			FAGAttributeInfo info = mAttributeInfo->FindAttributeInfoByTag(gameplayTags.Attributes_Primary_Coin);
	//			info.mAttributeValue = as->GetmCoin();
	//			info.mAttributeTag = gameplayTags.Attributes_Primary_Coin;
	//			mAttributeInfoDelegate.Broadcast(info);
	//		}
	//);






	/*mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
		as->GetmArmorAttribute()).AddLambda(
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

	mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
		as->GetmCriticalPercentAttribute()).AddLambda(
			[this, pair, as](const FOnAttributeChangeData& Data)
			{
				FAGAttributeInfo info = mAttributeInfo->FindAttributeInfoByTag(pair.Key);
				info.mAttributeValue = pair.Value.Execute().GetNumericValue(as);
				info.mAttributeTag = pair.Key;
				mAttributeInfoDelegate.Broadcast(info);
			}
	);

	mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
		as->GetmCriticalResistanceAttribute()).AddLambda(
			[this, pair, as](const FOnAttributeChangeData& Data)
			{
				FAGAttributeInfo info = mAttributeInfo->FindAttributeInfoByTag(pair.Key);
				info.mAttributeValue = pair.Value.Execute().GetNumericValue(as);
				info.mAttributeTag = pair.Key;
				mAttributeInfoDelegate.Broadcast(info);
			}
	);

	mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
		as->GetmCriticalDamageAttribute()).AddLambda(
			[this, pair, as](const FOnAttributeChangeData& Data)
			{
				FAGAttributeInfo info = mAttributeInfo->FindAttributeInfoByTag(pair.Key);
				info.mAttributeValue = pair.Value.Execute().GetNumericValue(as);
				info.mAttributeTag = pair.Key;
				mAttributeInfoDelegate.Broadcast(info);
			}
	);

	mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
		as->GetmMaxHpAttribute()).AddLambda(
			[this, pair, as](const FOnAttributeChangeData& Data)
			{
				FAGAttributeInfo info = mAttributeInfo->FindAttributeInfoByTag(pair.Key);
				info.mAttributeValue = pair.Value.Execute().GetNumericValue(as);
				info.mAttributeTag = pair.Key;
				mAttributeInfoDelegate.Broadcast(info);
			}
	);

	mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
		as->GetmMaxMpAttribute()).AddLambda(
			[this, pair, as](const FOnAttributeChangeData& Data)
			{
				FAGAttributeInfo info = mAttributeInfo->FindAttributeInfoByTag(pair.Key);
				info.mAttributeValue = pair.Value.Execute().GetNumericValue(as);
				info.mAttributeTag = pair.Key;
				mAttributeInfoDelegate.Broadcast(info);
			}
	);*/

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
			as->GetmIntelligenceAttribute()).AddLambda(
				[this, pair, as](const FOnAttributeChangeData& Data)
				{
					FAGAttributeInfo info = mAttributeInfo->FindAttributeInfoByTag(pair.Key);
					info.mAttributeValue = pair.Value.Execute().GetNumericValue(as);
					info.mAttributeTag = pair.Key;
					mAttributeInfoDelegate.Broadcast(info);
				}
		);

		mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
			as->GetmResilienceAttribute()).AddLambda(
				[this, pair, as](const FOnAttributeChangeData& Data)
				{
					FAGAttributeInfo info = mAttributeInfo->FindAttributeInfoByTag(pair.Key);
					info.mAttributeValue = pair.Value.Execute().GetNumericValue(as);
					info.mAttributeTag = pair.Key;
					mAttributeInfoDelegate.Broadcast(info);
				}
		);

		mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
			as->GetmCoinAttribute()).AddLambda(
				[this, pair, as](const FOnAttributeChangeData& Data)
				{
					FAGAttributeInfo info = mAttributeInfo->FindAttributeInfoByTag(pair.Key);
					info.mAttributeValue = pair.Value.Execute().GetNumericValue(as);
					info.mAttributeTag = pair.Key;
					mAttributeInfoDelegate.Broadcast(info);
				}
		);






		mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
			as->GetmArmorAttribute()).AddLambda(
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

		mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
			as->GetmCriticalPercentAttribute()).AddLambda(
				[this, pair, as](const FOnAttributeChangeData& Data)
				{
					FAGAttributeInfo info = mAttributeInfo->FindAttributeInfoByTag(pair.Key);
					info.mAttributeValue = pair.Value.Execute().GetNumericValue(as);
					info.mAttributeTag = pair.Key;
					mAttributeInfoDelegate.Broadcast(info);
				}
		);

		mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
			as->GetmCriticalResistanceAttribute()).AddLambda(
				[this, pair, as](const FOnAttributeChangeData& Data)
				{
					FAGAttributeInfo info = mAttributeInfo->FindAttributeInfoByTag(pair.Key);
					info.mAttributeValue = pair.Value.Execute().GetNumericValue(as);
					info.mAttributeTag = pair.Key;
					mAttributeInfoDelegate.Broadcast(info);
				}
		);

		mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
			as->GetmCriticalDamageAttribute()).AddLambda(
				[this, pair, as](const FOnAttributeChangeData& Data)
				{
					FAGAttributeInfo info = mAttributeInfo->FindAttributeInfoByTag(pair.Key);
					info.mAttributeValue = pair.Value.Execute().GetNumericValue(as);
					info.mAttributeTag = pair.Key;
					mAttributeInfoDelegate.Broadcast(info);
				}
		);

		mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
			as->GetmMaxHpAttribute()).AddLambda(
				[this, pair, as](const FOnAttributeChangeData& Data)
				{
					FAGAttributeInfo info = mAttributeInfo->FindAttributeInfoByTag(pair.Key);
					info.mAttributeValue = pair.Value.Execute().GetNumericValue(as);
					info.mAttributeTag = pair.Key;
					mAttributeInfoDelegate.Broadcast(info);
				}
		);

		mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(
			as->GetmMaxMpAttribute()).AddLambda(
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
