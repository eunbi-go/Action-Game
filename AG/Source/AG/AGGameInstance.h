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

	friend class UInventoryManager;


public:
	virtual void Init();

public:
	const FPlayerTableInfo* FindPlayerTable(const FName& _name);
	FPlayerTableInfo* FindPlayerTables(const FName& _name);
	const FMonsterTableInfo* FindMonsterTable(const FName& _name);
	const FSkillData* FindMonsterSkillTable(const FName& _name);
	const FItemDataTable* FindItemInfoTable(const FName& _name);
	const FItemAsset* FindItemAssetTable(const FName& _name);


private:
	UDataTable* mPlayerInfoTable;
	UDataTable* mMonsterInfoTable;
	UDataTable* mMonsterSkillTable;
	UDataTable* mItemInfoTable;
	UDataTable* mItemAssetTable;

	class UInventoryManager* mInventory;
};
