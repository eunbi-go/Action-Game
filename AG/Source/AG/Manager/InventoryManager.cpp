// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManager.h"

#include "../AGGameInstance.h"
#include "../AGGameModeBase.h"
#include "../Widget/MainWidget.h"
#include "../Widget/InventoryWidget.h"
#include "../Basic/ItemActor.h"
#include "../Widget/HUD/AGHUD.h"

UWorld* UInventoryManager::mWorld = nullptr;


UInventoryManager::UInventoryManager()
{
	mPreTime = 0;
}

UInventoryManager::~UInventoryManager()
{
}

UInventoryManager* UInventoryManager::GetInst(UWorld* _World)
{
	mWorld = _World;

	UAGGameInstance* gameInst = Cast<UAGGameInstance>(UGameplayStatics::GetGameInstance(_World));

	// �κ��丮�� ó������ ��û�� ����
	if (gameInst->mInventory == nullptr)
	{
		gameInst->mInventory = NewObject<UInventoryManager>();
		gameInst->mInventory->AddToRoot();
	}

	return gameInst->mInventory;
}

UInventoryManager* UInventoryManager::GetInst(UGameInstance* _GameInst)
{
	UAGGameInstance* gameInst = Cast<UAGGameInstance>(_GameInst);

	// �κ��丮�� ó������ ��û�� ����
	if (nullptr == gameInst->mInventory)
	{
		gameInst->mInventory = NewObject<UInventoryManager>();
		gameInst->mInventory->AddToRoot();
	}

	return gameInst->mInventory;
}

void UInventoryManager::InventoryOnOff(bool _Show)
{
	// ������ ȣ���� �ڿ� ���� �ð��� �������� üũ
	int32 curTime = 0;
	float curPartial = 0.f;
	UGameplayStatics::GetAccurateRealTime(curTime, curPartial);
	// �ð����̰� 1�ʸ� �Ѿ�� ���ϸ� ��ȯ
	if (curTime - mPreTime < 1)
		return;
	mPreTime = curTime;



	AAGGameModeBase* gameMode = Cast<AAGGameModeBase>(UGameplayStatics::GetGameMode(mWorld));
	if (nullptr == gameMode)
		return;

	AAGHUD* hud = Cast<AAGHUD>(mWorld->GetFirstPlayerController()->GetHUD());
	if (!IsValid(hud))
		return;
	UMainWidget* mainWidget = hud->mMainWidget;
	if (!IsValid(mainWidget))
		return;
	UInventoryWidget* inventoryWidget = mainWidget->GetInventoryWidget();
	if (!IsValid(inventoryWidget))
		return;

	// �κ��丮 ��
	if (_Show)
	{
		inventoryWidget->SetVisibility(ESlateVisibility::Visible);

		APlayerController* Controller = mWorld->GetFirstPlayerController();
		
		FInputModeGameAndUI uionly;
		//FInputModeUIOnly uionly;
		Controller->SetInputMode(uionly);
		Controller->bShowMouseCursor = true;
	}

	// �κ��丮 ����
	else
	{
		inventoryWidget->SetVisibility(ESlateVisibility::Collapsed);

		APlayerController* Controller = mWorld->GetFirstPlayerController();
		FInputModeGameOnly gameonly;
		Controller->SetInputMode(gameonly);
		Controller->bShowMouseCursor = false;
	}
}

bool UInventoryManager::IsInventoryOn()
{
	AAGGameModeBase* GameMode = Cast<AAGGameModeBase>(UGameplayStatics::GetGameMode(mWorld));
	if (nullptr == GameMode)
		return false;

	AAGHUD* hud = Cast<AAGHUD>(mWorld->GetFirstPlayerController()->GetHUD());
	if (!IsValid(hud))
		return false;
	UMainWidget* mainWidget = hud->mMainWidget;
	if (!IsValid(mainWidget))
		return false;
	UInventoryWidget* inventoryWidget = mainWidget->GetInventoryWidget();
	if (!IsValid(inventoryWidget))
		return false;

	return inventoryWidget->IsVisible();
}

void UInventoryManager::SetItemInfoTable(UDataTable* _Table)
{
	mItemTable = _Table;

	// ������ ���̺� ����ִ� ��� ����ü ����
	FString str;
	TArray<FItemDataTable2*> AllItemInfo;
	mItemTable->GetAllRows<FItemDataTable2>(str, AllItemInfo);

	// ������ ���̺� ����ִ� ��� �� �̸�
	TArray<FName> AllRowname;
	AllRowname = mItemTable->GetRowNames();

	// ���̺������� �� �޾ƿͼ� ��ü �ݺ����� ����.
	// ItemID �� Ű������, �ش� �������� RowName �� �����ͷ� �����Ѵ�.
	for (int i = 0; i < AllItemInfo.Num(); ++i)
	{
		mRownameMap.Add(AllItemInfo[i]->id, AllRowname[i]);
	}
}

const FItemDataTable2* UInventoryManager::GetItemInfo(EITEM_ID _ItemID)
{
	// ItemID �� �����ص� RowName �� ã�Ƽ�
	FName RowName = mRownameMap.FindRef(_ItemID);

	// ������ ���̺��� �˻��� �Ѵ�.
	FItemDataTable2* ItemInfo = mItemTable->FindRow<FItemDataTable2>(RowName, nullptr);

	return ItemInfo;
}
