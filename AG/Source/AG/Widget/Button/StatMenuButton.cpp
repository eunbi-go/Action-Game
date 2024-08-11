// Fill out your copyright notice in the Description page of Project Settings.


#include "StatMenuButton.h"
#include "../Stat/StatWidget.h"
#include "../Button/AGButton.h"
#include "../WidgetController/AttributeWidgetController.h"
#include "../../AbilitySystem/AGAbilitySystemLibrary.h"
#include "../HUD/AGHUD.h"
#include "../MainWidget.h"
#include "../../Player/AGPlayerController.h"

void UStatMenuButton::NativeConstruct()
{
	Super::NativeConstruct();

	mButton = Cast<UAGButton>(GetWidgetFromName(FName(TEXT("UI_AGButton"))));
	mButton->GetButton()->OnClicked.AddDynamic(this, &UStatMenuButton::ButtonClicked);

	mAttributeWidgetControllerClass = UAttributeWidgetController::StaticClass();
}

void UStatMenuButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UStatMenuButton::ButtonClicked()
{
	PrintViewport(3.f, FColor::Yellow, FString("Clicked StatMenu Button"));


	mButton->GetButton()->SetIsEnabled(false);

	FStringClassReference MyWidgetClassRef(TEXT("WidgetBlueprint'/Game/Blueprints/UMG/AttributeMenu/UI_StatMenu.UI_StatMenu_C'"));

	if (UClass* MyWidgetClass = MyWidgetClassRef.TryLoadClass<UUserWidget>())
	{
		mStatWidget = CreateWidget<UStatWidget>(GetWorld(), MyWidgetClass);
		mStatWidget->AddToViewport();
		mStatWidget->SetPositionInViewport(FVector2D(50.f, 50.f));

		mAttributeWidgetController = UAGAbilitySystemLibrary::GetAttributeWidgetController(this);
		mStatWidget->SetWidgetController(mAttributeWidgetController);

		APlayerController* controller = UGameplayStatics::GetPlayerController(this, 0);
		Cast<AAGPlayerController>(controller)->bShowMouseCursor = true;

		mStatWidget->mOnCloseButtonClickedDelegate.AddLambda([this]()
			{
				mButton->GetButton()->SetIsEnabled(true);
				APlayerController* controller = UGameplayStatics::GetPlayerController(this, 0);
				Cast<AAGPlayerController>(controller)->bShowMouseCursor = false;
			});


	}

}
