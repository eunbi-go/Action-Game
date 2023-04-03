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
	virtual void PostLogin(APlayerController* NewPlayer);
	virtual void Tick(float DeltaTime)	override;

public:
	class UMainWidget* GetMainWidget() { return mMainWidget; }


private:
	TSubclassOf<UUserWidget>	mMainWidgetClass;
	class UMainWidget*			mMainWidget;
};
