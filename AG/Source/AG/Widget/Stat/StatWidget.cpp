// Fill out your copyright notice in the Description page of Project Settings.


#include "StatWidget.h"
#include "../Button/AGButton.h"
#include "../Button/CloseButton.h"
#include "../WidgetController/AttributeWidgetController.h"
#include "AttributeValueRow.h"
#include "../../AbilitySystem/AGAbilitySystemLibrary.h"
#include "../../AGGameplayTags.h"
#include "AGAttributeValueButtonRow.h"

void UStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mCloseButton = Cast<UCloseButton>(GetWidgetFromName(FName(TEXT("CloseButton"))));
	mCloseButton->mButton->GetButton()->OnClicked.AddDynamic(this, &UStatWidget::CloseButtonClicked);

	mStrengthRow = Cast<UAGAttributeValueButtonRow>(GetWidgetFromName(FName(TEXT("First_Stength"))));
	mDexterityRow = Cast<UAGAttributeValueButtonRow>(GetWidgetFromName(FName(TEXT("First_Dexterity"))));
	mIntelligenceRow = Cast<UAGAttributeValueButtonRow>(GetWidgetFromName(FName(TEXT("First_Intelligence"))));
	mResilienceRow = Cast<UAGAttributeValueButtonRow>(GetWidgetFromName(FName(TEXT("First_Resilience"))));
	mCoinRow = Cast<UAttributeValueRow>(GetWidgetFromName(FName(TEXT("First_Coin"))));

	mArmorRow = Cast<UAttributeValueRow>(GetWidgetFromName(FName(TEXT("Second_Armor"))));
	mBlockPercentRow = Cast<UAttributeValueRow>(GetWidgetFromName(FName(TEXT("Second_BlockPercent"))));
	mCriticalPercentRow = Cast<UAttributeValueRow>(GetWidgetFromName(FName(TEXT("Second_CriticalPercent"))));
	mCriticalResistanceRow = Cast<UAttributeValueRow>(GetWidgetFromName(FName(TEXT("Second_CriticalResistance"))));
	mCriticalDamageRow = Cast<UAttributeValueRow>(GetWidgetFromName(FName(TEXT("Second_CriticalDamage"))));
	mMaxHpRow = Cast<UAttributeValueRow>(GetWidgetFromName(FName(TEXT("Second_MaxHp"))));
	mMaxMpRow = Cast<UAttributeValueRow>(GetWidgetFromName(FName(TEXT("Second_MaxMp"))));
	

	// Attribute Row에 Tag 할당
	mStrengthRow->mRow->mTag = FAGGameplayTags::Get().Attributes_Primary_Strength;
	mDexterityRow->mRow->mTag = FAGGameplayTags::Get().Attributes_Primary_Dexterity;
	mIntelligenceRow->mRow->mTag = FAGGameplayTags::Get().Attributes_Primary_Intelligence;
	mResilienceRow->mRow->mTag = FAGGameplayTags::Get().Attributes_Primary_Resilience;
	mCoinRow->mTag = FAGGameplayTags::Get().Attributes_Primary_Coin;

	mArmorRow->mTag = FAGGameplayTags::Get().Attributes_Second_Armor;
	mBlockPercentRow->mTag = FAGGameplayTags::Get().Attributes_Second_BlockPercent;
	mCriticalPercentRow->mTag = FAGGameplayTags::Get().Attributes_Second_CriticalPercent;
	mCriticalResistanceRow->mTag = FAGGameplayTags::Get().Attributes_Second_CriticalResistance;
	mCriticalDamageRow->mTag = FAGGameplayTags::Get().Attributes_Second_CriticalDamage;
	mMaxHpRow->mTag = FAGGameplayTags::Get().Attributes_Second_MaxHp;
	mMaxMpRow->mTag = FAGGameplayTags::Get().Attributes_Second_MaxMp;

	// WidgetController 할당
	mWidgetController = UAGAbilitySystemLibrary::GetAttributeWidgetController(GetWorld());
}

void UStatWidget::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);
}



void UStatWidget::CloseButtonClicked()
{
	PrintViewport(3.f, FColor::Yellow, FString("UStatWidget Close Button"));

	mOnCloseButtonClickedDelegate.Broadcast();
	RemoveFromParent();
}

void UStatWidget::SetWidgetController(UAttributeWidgetController* widgetController)
{
	mWidgetController = widgetController;
	checkf(mWidgetController, TEXT("UStatWidget::SetWidgetController() widgetController : nullptr"))

	Cast<UAttributeWidgetController>(mWidgetController)->BroadcastInitValues();
	Cast<UAttributeWidgetController>(mWidgetController)->BindCallbacksToDependecies();
}
