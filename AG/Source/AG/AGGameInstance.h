// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BasicInfo.h"
#include "Engine/GameInstance.h"
#include "AGGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class AG_API UAGGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UAGGameInstance();
	~UAGGameInstance();



public:
	virtual void Init();

public:
	const FPlayerTableInfo* FindPlayerTable(const FName& _name);
	const FMonsterTableInfo* FindMonsterTable(const FName& _name);




private:
	UDataTable* mPlayerInfoTable;
	UDataTable* mMonsterInfoTable;
};
