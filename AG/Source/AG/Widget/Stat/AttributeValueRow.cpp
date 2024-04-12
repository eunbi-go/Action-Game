// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeValueRow.h"
#include "../../AbilitySystem/AGAbilitySystemLibrary.h"
#include "../WidgetController/AttributeWidgetController.h"

void UAttributeValueRow::NativeConstruct()
{
	Super::NativeConstruct();

	mAttributeName = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_AttributeName")));
	mAttributeValue = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextBlock_AttributeValue")));

	UAGAbilitySystemLibrary::GetAttributeWidgetController(GetWorld())->mAttributeInfoDelegate.AddDynamic(this, &UAttributeValueRow::ValueChanged);
}

void UAttributeValueRow::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UAttributeValueRow::ValueChanged(const FAGAttributeInfo& Info)
{
	if (Info.mAttributeTag.MatchesTag(mTag))
	{
		mAttributeName->SetText(Info.mAttributeName);
		FString valueString = FString::SanitizeFloat(Info.mAttributeValue);
		mAttributeValue->SetText(FText::FromString(valueString));
	}
}
