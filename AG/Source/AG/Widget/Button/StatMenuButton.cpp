// Fill out your copyright notice in the Description page of Project Settings.


#include "StatMenuButton.h"
#include "../Stat/StatWidget.h"
#include "../Button/AGButton.h"

void UStatMenuButton::NativeConstruct()
{
	Super::NativeConstruct();

	mButton = Cast<UAGButton>(GetWidgetFromName(FName(TEXT("UI_AGButton"))));
	mButton->GetButton()->OnClicked.AddDynamic(this, &UStatMenuButton::ButtonClicked);
}

void UStatMenuButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UStatMenuButton::ButtonClicked()
{
	PrintViewport(3.f, FColor::Yellow, FString("Clicked StatMenu Button"));

	if (mButton->mButtonState == BUTTON_STATE::UNCLICKED)
	{
		mButton->GetButton()->SetIsEnabled(false);
		mButton->mButtonState = BUTTON_STATE::CLICKED;

		FStringClassReference MyWidgetClassRef(TEXT("WidgetBlueprint'/Game/Blueprints/UMG/AttributeMenu/UI_StatMenu.UI_StatMenu_C'"));

		if (UClass* MyWidgetClass = MyWidgetClassRef.TryLoadClass<UUserWidget>())
		{
			mStatWidget = CreateWidget<UStatWidget>(GetWorld(), MyWidgetClass);
			mStatWidget->AddToViewport();
			mStatWidget->SetPositionInViewport(FVector2D(50.f, 50.f));

			mStatWidget->mOnCloseButtonClickedDelegate.AddLambda([this]()
				{
					mButton->GetButton()->SetIsEnabled(true);
				});
		}
	}
	else if (mButton->mButtonState == BUTTON_STATE::CLICKED)
	{
		mButton->mButtonState = BUTTON_STATE::UNCLICKED;
	}
}
