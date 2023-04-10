// Copyright Epic Games, Inc. All Rights Reserved.


#include "AGGameModeBase.h"

#include "Player/WarriorCharacter.h"
#include "Player/AGPlayerController.h"
#include "Widget/MainWidget.h"

#include "AGGameInstance.h"

AAGGameModeBase::AAGGameModeBase()
{
	DefaultPawnClass = AWarriorCharacter::StaticClass();
	PlayerControllerClass = AAGPlayerController::StaticClass();

	ConstructorHelpers::FClassFinder<UUserWidget>
		mainWidget(TEXT("WidgetBlueprint'/Game/Blueprints/UMG/UI_Main.UI_Main_C'"));

	if (mainWidget.Succeeded())
	{
		mMainWidgetClass = mainWidget.Class;
	}
}

void AAGGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// 위젯 블루프린트 UClass 정보를 이용해서 객체를 만들어낸다.
	if (IsValid(mMainWidgetClass))
	{
		// 생성한 객체의 주소를 m_MainHUD 에 받아둔다.
		mMainWidget = Cast<UMainWidget>(CreateWidget(GetWorld(), mMainWidgetClass));
		if (IsValid(mMainWidget))
		{
			mMainWidget->AddToViewport();
			mMainWidget->SetHp(1.f);
		}
	}
}

void AAGGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

}

void AAGGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
