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
	


	mFirstAttributeScrollBox = Cast<UScrollBox>(GetWidgetFromName(FName(TEXT("ScrollBox_FirstAttributes"))));
	mDexterityRow = Cast<UAGAttributeValueButtonRow>(GetWidgetFromName(FName(TEXT("First_Strength"))));
	mBlockPercentRow = Cast<UAttributeValueRow>(GetWidgetFromName(FName(TEXT("Second_BlockPercent"))));
	//mIntelligenceRow = Cast<UAttributeValueRow>(GetWidgetFromName(FName(TEXT("Row_Intelligence"))));

	

	//mFirstAttributeScrollBox->AddChild(mStrengthRow);
	//mFirstAttributeScrollBox->AddChild(mIntelligenceRow);


	// Set WidgetController
	mWidgetController = UAGAbilitySystemLibrary::GetAttributeWidgetController(GetWorld());

	mBlockPercentRow->mTag = FAGGameplayTags::Get().Attributes_Second_BlockPercent;
	mDexterityRow->mRow->mTag = FAGGameplayTags::Get().Attributes_Primary_Dexterity;

	// Attribute Row¿¡ Tag ÇÒ´ç
	SetAttributeTag();

	

	Cast<UAttributeWidgetController>(mWidgetController)->BroadcastInitValues();
}

void UStatWidget::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);
}

void UStatWidget::SetAttributeTag()
{
	mBlockPercentRow->mTag = FAGGameplayTags::Get().Attributes_Second_BlockPercent;
	mDexterityRow->mRow->mTag = FAGGameplayTags::Get().Attributes_Primary_Dexterity;
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
}
