// Fill out your copyright notice in the Description page of Project Settings.


#include "AGHUD.h"
#include "../MainWidget.h"
#include "../WidgetController/MainWidgetController.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "../WidgetController/AttributeWidgetController.h"
#include "../Stat/StatWidget.h"

AAGHUD::AAGHUD()
{
	//mMainWidgetClass = UMainWidget::StaticClass();
	mMainWidgetControllerClass = UMainWidgetController::StaticClass();
	static ConstructorHelpers::FClassFinder<UUserWidget> ribbonMontage(TEXT("WidgetBlueprint'/Game/Blueprints/UMG/UI_Main.UI_Main_C'"));
	if (ribbonMontage.Succeeded())
	{
		mMainWidgetClass = ribbonMontage.Class;
	}

	//static ConstructorHelpers::FClassFinder<UMainWidgetController> mwdc(TEXT("Class'/Script/AG.MainWidgetController'"));
	//if (mwdc.Succeeded())
	//{
	//	mMainWidgetControllerClass = mwdc.Class;
	//}

	mAttributeWidgetControllerClass = UAttributeWidgetController::StaticClass();
	mStatWidgetClass = UStatWidget::StaticClass();
}

UMainWidgetController* AAGHUD::GetMainWidgetController(const FWidgetControllerParams& params)
{
	if (!mMainWidgetController)
	{
		mMainWidgetController = NewObject<UMainWidgetController>(this, mMainWidgetControllerClass);
		mMainWidgetController->SetWidgetControllerParams(params);
		mMainWidgetController->BindCallbacksToDependecies();

		return mMainWidgetController;
	}

	return mMainWidgetController;
}

UAttributeWidgetController* AAGHUD::GetAttributeWidgetController(const FWidgetControllerParams& params)
{
	if (!mAttributeWidgetController)
	{
		mAttributeWidgetController = NewObject<UAttributeWidgetController>(this, mAttributeWidgetControllerClass);
		mAttributeWidgetController->SetWidgetControllerParams(params);
		mAttributeWidgetController->BindCallbacksToDependecies();

		return mAttributeWidgetController;
	}
	return mAttributeWidgetController;
}

void AAGHUD::InitMainWidget(APlayerController* pc, APlayerState* ps, UAbilitySystemComponent* asc, UAttributeSet* as)
{
	checkf(mMainWidgetClass, TEXT("MainWidgetClass uninitialized"));
	checkf(mMainWidgetControllerClass, TEXT("MainWidgetControllerClass uninitialized"));

	UMainWidget* widget = CreateWidget<UMainWidget>(GetWorld(), mMainWidgetClass);
	mMainWidget = Cast<UMainWidget>(widget);
	
	const FWidgetControllerParams params(pc, ps, asc, as);
	mMainWidgetController = GetMainWidgetController(params);

	mMainWidget->SetWidgetController(Cast<UMainWidgetController>(mMainWidgetController));
	mMainWidgetController->BroadcastInitValues();

	widget->AddToViewport();
}

void AAGHUD::BeginPlay()
{
	Super::BeginPlay();
	//PrintViewport(3.f, FColor::Red, TEXT("BeginPlay"));
	//mMainWidgetController->BroadcastInitValues();
}
