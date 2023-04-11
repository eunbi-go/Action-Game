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
