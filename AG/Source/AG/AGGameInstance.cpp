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
	// 2��° ������ ContextString�� �ش� Ű�� �̿��Ͽ� Ž����
	// ������ ��� ����� �޽����� FString Ÿ������ �����Ѵ�.
	return mPlayerInfoTable->FindRow<FPlayerTableInfo>(_name, TEXT(""));
}