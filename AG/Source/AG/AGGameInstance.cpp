// Fill out your copyright notice in the Description page of Project Settings.


#include "AGGameInstance.h"

UAGGameInstance::UAGGameInstance()
{
	mPlayerInfoTable = nullptr;
	mMonsterInfoTable = nullptr;


	static ConstructorHelpers::FObjectFinder<UDataTable>	playerTable(TEXT("DataTable'/Game/Blueprints/DataTable/PlayerInfoTable.PlayerInfoTable'"));
	if (playerTable.Succeeded())
		mPlayerInfoTable = playerTable.Object;


	static ConstructorHelpers::FObjectFinder<UDataTable>	monsterTable(TEXT("DataTable'/Game/Blueprints/DataTable/MonsterInfoTable.MonsterInfoTable'"));
	if (monsterTable.Succeeded())
		mMonsterInfoTable = monsterTable.Object;


	static ConstructorHelpers::FObjectFinder<UDataTable>	monsterSkillTable(TEXT("DataTable'/Game/Blueprints/DataTable/SkillInfoTable.SkillInfoTable'"));
	if (monsterSkillTable.Succeeded())
		mMonsterSkillTable = monsterSkillTable.Object;
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
	// 2��° ������ ContextString�� �ش� Ű�� �̿��Ͽ� Ž����
	// ������ ��� ����� �޽����� FString Ÿ������ �����Ѵ�.
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
