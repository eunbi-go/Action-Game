// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AGGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class AG_API AAGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AAGGameModeBase();

public:
	virtual void BeginPlay()	override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);

	virtual void PostLogin(APlayerController* NewPlayer);
	virtual void Tick(float DeltaTime)	override;

public:
	class UMainWidget* GetMainWidget() { return mMainWidget; }
	class UAGSaveGame* GetSaveGame() { return mSaveGame; }


private:
	void SaveGame();


private:
	TSubclassOf<UUserWidget>	mMainWidgetClass;
	class UMainWidget*			mMainWidget;
	class UAGSaveGame*		mSaveGame;
};
