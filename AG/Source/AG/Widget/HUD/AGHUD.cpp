// Fill out your copyright notice in the Description page of Project Settings.


#include "AGHUD.h"
#include "../MainWidget.h"
#include "../WidgetController/MainWidgetController.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"

AAGHUD::AAGHUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> ribbonMontage(TEXT("WidgetBlueprint'/Game/Blueprints/UMG/UI_Main.UI_Main_C'"));
	if (ribbonMontage.Succeeded())
	{
		mMainWidgetClass = ribbonMontage.Class;
	}

	static ConstructorHelpers::FClassFinder<UMainWidgetController> mwdc(TEXT("Class'/Script/AG.MainWidgetController'"));
	if (mwdc.Succeeded())
	{
		mMainWidgetControllerClass = mwdc.Class;
	}
}

UMainWidgetController* AAGHUD::GetMainWidgetController(const FWidgetControllerParams& params)
{
	if (!mMainWidgetController)
	{
		mMainWidgetController = NewObject<UMainWidgetController>(this, mMainWidgetControllerClass);
		mMainWidgetController->SetWidgetControllerParams(params);

		return mMainWidgetController;
	}

	return mMainWidgetController;
}

void AAGHUD::InitMainWidget(APlayerController* pc, APlayerState* ps, UAbilitySystemComponent* asc, UAttributeSet* as)
{
	checkf(mMainWidgetClass, TEXT("MainWidgetClass uninitialized"));
	checkf(mMainWidgetControllerClass, TEXT("MainWidgetControllerClass uninitialized"));

	UUserWidget* widget = CreateWidget<UUserWidget>(GetWorld(), mMainWidgetClass);
	mMainWidget = Cast<UMainWidget>(widget);
	
	const FWidgetControllerParams params(pc, ps, asc, as);
	mMainWidgetController = GetMainWidgetController(params);

	mMainWidget->SetWidgetController(Cast<UMainWidgetController>(mMainWidgetController));
	
	widget->AddToViewport();
}

void AAGHUD::BeginPlay()
{
	Super::BeginPlay();
	//PrintViewport(3.f, FColor::Red, TEXT("BeginPlay"));
	mMainWidgetController->BroadcastInitValues();
}
