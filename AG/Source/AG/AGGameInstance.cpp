// Fill out your copyright notice in the Description page of Project Settings.


#include "AGGameInstance.h"

UAGGameInstance::UAGGameInstance()
{
	mPlayerInfoTable = nullptr;

	static ConstructorHelpers::FObjectFinder<UDataTable>	playerTable(TEXT("DataTable'/Game/Blueprints/DataTable/PlayerInfoTable.PlayerInfoTable'"));
	if (playerTable.Succeeded())
		mPlayerInfoTable = playerTable.Object;
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