// Copyright Epic Games, Inc. All Rights Reserved.


#include "AGGameModeBase.h"
#include "Player/WarriorCharacter.h"
#include "Player/AGPlayerController.h"

AAGGameModeBase::AAGGameModeBase()
{
	DefaultPawnClass = AWarriorCharacter::StaticClass();
	PlayerControllerClass = AAGPlayerController::StaticClass();
}
