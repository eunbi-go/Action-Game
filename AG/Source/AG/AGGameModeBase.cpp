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

	// ���� �������Ʈ UClass ������ �̿��ؼ� ��ü�� ������.
	if (IsValid(mMainWidgetClass))
	{
		// ������ ��ü�� �ּҸ� m_MainHUD �� �޾Ƶд�.
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
