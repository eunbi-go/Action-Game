// Fill out your copyright notice in the Description page of Project Settings.


#include "AGGameInstance.h"
#include "Manager/InventoryManager.h"

UAGGameInstance::UAGGameInstance()
{
	mPlayerInfoTable = nullptr;
	mMonsterInfoTable = nullptr;
	mMonsterSkillTable = nullptr;
	mItemInfoTable = nullptr;


	static ConstructorHelpers::FObjectFinder<UDataTable>	playerTable(TEXT("DataTable'/Game/Blueprints/DataTable/PlayerInfoTable.PlayerInfoTable'"));
	if (playerTable.Succeeded())
	{
		mPlayerInfoTable = playerTable.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable>	monsterTable(TEXT("DataTable'/Game/Blueprints/DataTable/MonsterInfoTable.MonsterInfoTable'"));
	if (monsterTable.Succeeded())
		mMonsterInfoTable = monsterTable.Object;


	static ConstructorHelpers::FObjectFinder<UDataTable>	monsterSkillTable(TEXT("DataTable'/Game/Blueprints/DataTable/MonsterSkillInfoTable.MonsterSkillInfoTable'"));
	if (monsterSkillTable.Succeeded())
		mMonsterSkillTable = monsterSkillTable.Object;


	static ConstructorHelpers::FObjectFinder<UDataTable>	itemInfoTable(TEXT("DataTable'/Game/Blueprints/DataTable/ItemInfoTable.ItemInfoTable'"));
	if (itemInfoTable.Succeeded())
	{
		mItemInfoTable = itemInfoTable.Object;
		UInventoryManager::GetInst(this)->SetItemInfoTable(mItemInfoTable);
	}


	static ConstructorHelpers::FObjectFinder<UDataTable>	itemAssetTable(TEXT("DataTable'/Game/Blueprints/DataTable/ItemAssetTable.ItemAssetTable'"));
	if (itemAssetTable.Succeeded())
	{
		mItemAssetTable = itemAssetTable.Object;
	}


	static ConstructorHelpers::FObjectFinder<UDataTable>	messageDataTable(TEXT("DataTable'/Game/Blueprints/UMG/Data/DT_MessageWidgetData.DT_MessageWidgetData'"));
	if (messageDataTable.Succeeded())
	{
		mMessageDataTable = messageDataTable.Object;
	}
	
}

UAGGameInstance::~UAGGameInstance()
{
}

void UAGGameInstance::Init()
{
	Super::Init();
}

const FPlayerTableInfo* UAGGameInstance::FindPlayerTable(const FName& _name)
{
	// 2번째 인자인 ContextString은 해당 키를 이용하여 탐색이
	// 실패할 경우 출력할 메시지를 FString 타입으로 전달한다.
	return mPlayerInfoTable->FindRow<FPlayerTableInfo>(_name, TEXT(""));
}
FPlayerTableInfo* UAGGameInstance::FindPlayerTables(const FName& _name)
{
	return mPlayerInfoTable->FindRow<FPlayerTableInfo>(_name, TEXT(""));
}

const FMonsterTableInfo* UAGGameInstance::FindMonsterTable(const FName& _name)
{
	return mMonsterInfoTable->FindRow<FMonsterTableInfo>(_name, TEXT(""));
}

const FSkillData* UAGGameInstance::FindMonsterSkillTable(const FName& _name)
{
	return mMonsterSkillTable->FindRow<FSkillData>(_name, TEXT(""));
}

const FItemDataTable* UAGGameInstance::FindItemInfoTable(const FName& _name)
{
	return mItemInfoTable->FindRow<FItemDataTable>(_name, TEXT(""));
}

const FItemAsset* UAGGameInstance::FindItemAssetTable(const FName& _name)
{
	return mItemAssetTable->FindRow<FItemAsset>(_name, TEXT(""));
}
