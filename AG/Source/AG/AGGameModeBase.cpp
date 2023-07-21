// Copyright Epic Games, Inc. All Rights Reserved.


#include "AGGameModeBase.h"

#include "Player/WarriorCharacter.h"
#include "Player/AGPlayerController.h"
#include "Player/AGPlayer.h"
#include "Player/Valkyrie.h"
#include "Widget/MainWidget.h"
#include "AGGameInstance.h"
#include "AGSaveGame.h"


AAGGameModeBase::AAGGameModeBase()
{
	DefaultPawnClass = AValkyrie::StaticClass();
	//PlayerControllerClass = AAGPlayerController::StaticClass();

	//ConstructorHelpers::FClassFinder<UUserWidget>
	//	mainWidget(TEXT("WidgetBlueprint'/Game/Blueprints/UMG/UI_Main.UI_Main_C'"));

	//if (mainWidget.Succeeded())
	//{
	//	mMainWidgetClass = mainWidget.Class;
	//}
}

void AAGGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	mSaveGame = NewObject<UAGSaveGame>();

	if (IsValid(mMainWidgetClass))
	{
		//mMainWidget = Cast<UMainWidget>(CreateWidget(GetWorld(), mMainWidgetClass));
		//if (IsValid(mMainWidget))
		//{
		//	mMainWidget->AddToViewport();
		//	mMainWidget->SetHp(1.f);
		//}
	}
}

void AAGGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	/*switch (EndPlayReason)
	{
	case EEndPlayReason::Destroyed:
		LOG(TEXT("Destroyed"));
		break;

	case EEndPlayReason::LevelTransition:
		LOG(TEXT("Level Transition"));
		SaveGame();
		break;

	case EEndPlayReason::EndPlayInEditor:
		LOG(TEXT("EndPlayInEditor"));
		break;

	case EEndPlayReason::RemovedFromWorld:
		LOG(TEXT("RemovedFromWorld"));
		break;

	case EEndPlayReason::Quit:
		LOG(TEXT("Quit"));
		break;
	}*/
}

void AAGGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	//UAGGameInstance* GameInst = GetWorld()->GetGameInstance<UAGGameInstance>();

	//if (IsValid(GameInst))
	//{
	//	DefaultPawnClass = AWarriorCharacter::StaticClass();
	//}
}

void AAGGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

}

void AAGGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAGGameModeBase::SaveGame()
{
	FString fullPath = FPaths::ProjectSavedDir() + TEXT("SaveGames/Save.txt");

	FArchive* writer = IFileManager::Get().CreateFileWriter(*fullPath);

	if (writer)
	{
		*writer << mSaveGame->mPlayerInfo.name;
		*writer << mSaveGame->mPlayerInfo.attackPoint;
		*writer << mSaveGame->mPlayerInfo.defensePoint;
		*writer << mSaveGame->mPlayerInfo.hp;
		*writer << mSaveGame->mPlayerInfo.maxHp;
		*writer << mSaveGame->mPlayerInfo.mp;
		*writer << mSaveGame->mPlayerInfo.maxMp;
		*writer << mSaveGame->mPlayerInfo.level;
		*writer << mSaveGame->mPlayerInfo.exp;
		*writer << mSaveGame->mPlayerInfo.gold;
		*writer << mSaveGame->mPlayerInfo.movingWalkSpeed;
		*writer << mSaveGame->mPlayerInfo.movingRunSpeed;
		*writer << mSaveGame->mPlayerInfo.movingDashSpeed;
		*writer << mSaveGame->mPlayerInfo.attackDistance;

		// 사용이 끝났다면 스트림을 닫아주고 제거해야 한다.
		writer->Close();

		delete writer;

		PrintViewport(10.f, FColor::Blue, TEXT("AAGGameModeBase::SaveGame"));
	}
}
