// Fill out your copyright notice in the Description page of Project Settings.


#include "StatButton.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Stat/StatWidget.h"
#include "../Player/AGPlayerController.h"
#include "WidgetController/MainWidgetController.h"

void UStatButton::NativeConstruct()
{
	Super::NativeConstruct();
	mButton = Cast<UButton>(GetWidgetFromName(TEXT("Button")));
	mSizeBox = Cast<USizeBox>(GetWidgetFromName(TEXT("SizeBox_Root")));
	mText = Cast<UTextBlock>(GetWidgetFromName(TEXT("Text_ButtonName")));

	mButton->OnClicked.AddDynamic(this, &UStatButton::StatButtonClicked);
}

void UStatButton::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
}

void UStatButton::StatButtonClicked()
{
	PrintViewport(3.f, FColor::Yellow, FString("Clicked Stat Button"));


	FStringClassReference MyWidgetClassRef(TEXT("WidgetBlueprint'/Game/Blueprints/UMG/AttributeMenu/UI_StatMenu.UI_StatMenu_C'"));

	if (UClass* MyWidgetClass = MyWidgetClassRef.TryLoadClass<UUserWidget>())
	{
		mStatWidget = CreateWidget<UStatWidget>(GetWorld(), MyWidgetClass);
		mStatWidget->AddToViewport();
		mStatWidget->SetPositionInViewport(FVector2D(50.f, 50.f));
		

		mButton->SetIsEnabled(false);
	}
}
