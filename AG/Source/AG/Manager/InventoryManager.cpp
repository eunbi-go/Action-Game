// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManager.h"

#include "../AGGameInstance.h"
#include "../AGGameModeBase.h"
#include "../Widget/MainWidget.h"
#include "../Widget/InventoryWidget.h"
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
	int32 CurTime = 0;
	float CurPartial = 0.f;
	UGameplayStatics::GetAccurateRealTime(CurTime, CurPartial);

	// �ð����̰� 1�ʸ� �Ѿ�� ���ϸ� ��ȯ
	if (CurTime - mPreTime < 1)
		return;

	mPreTime = CurTime;



	// ���� ���Ӹ�带 �˾Ƴ���. (�Է����� ���� ���尡 �䱸�ȴ�, ���� ������ ���Ӹ�带 �˾Ƴ��� �ϱ� ����)
	AAGGameModeBase* GameMode = Cast<AAGGameModeBase>(UGameplayStatics::GetGameMode(mWorld));

	// �˾Ƴ� ���Ӹ�尡 AAGGameModeBase �� �ƴ϶�� ĳ���� ���� == ���� ���尡 ���� ������ �ƴ϶�� ��
	if (nullptr == GameMode)
		return;


	// ���� ���Ӹ�尡 AAGGameModeBase �� �´ٸ�, MainHUD �� �����ؼ� InventoryWiget �� Visible ���θ� Ȯ���Ѵ�.
	UMainWidget* MainHUD = GameMode->GetMainWidget();
	UInventoryWidget* InveotyrWidget = MainHUD->GetInventoryWidget();

	// �κ��丮 ��
	if (_Show)
	{
		InveotyrWidget->SetVisibility(ESlateVisibility::Visible);

		APlayerController* Controller = mWorld->GetFirstPlayerController();

		FInputModeGameAndUI uionly;
		//FInputModeUIOnly uionly;
		Controller->SetInputMode(uionly);
		Controller->bShowMouseCursor = true;
	}

	// �κ��丮 ����
	else
	{
		
		InveotyrWidget->SetVisibility(ESlateVisibility::Collapsed);


		APlayerController* Controller = mWorld->GetFirstPlayerController();

		//FInputModeGameOnly gameonly;
		FInputModeGameAndUI gameonly;
		Controller->SetInputMode(gameonly);
		Controller->bShowMouseCursor = false;
	}
}

bool UInventoryManager::IsInventoryOn()
{
	// ���� ���Ӹ�带 �˾Ƴ���. (�Է����� ���� ���尡 �䱸�ȴ�, ���� ������ ���Ӹ�带 �˾Ƴ��� �ϱ� ����)
	AAGGameModeBase* GameMode = Cast<AAGGameModeBase>(UGameplayStatics::GetGameMode(mWorld));

	// �˾Ƴ� ���Ӹ�尡 AAGGameModeBase �� �ƴ϶�� ĳ���� ���� == ���� ���尡 ���� ������ �ƴ϶�� ��
	if (nullptr == GameMode)
		return false;

	// ���� ���Ӹ�尡 UAGGameInstance �� �´ٸ�, MainHUD �� �����ؼ� InventoryWiget �� Visible ���θ� Ȯ���Ѵ�.
	UMainWidget* MainHUD = GameMode->GetMainWidget();
	UInventoryWidget* InveotyrWidget = MainHUD->GetInventoryWidget();

	return InveotyrWidget->IsVisible();
}

void UInventoryManager::SetItemInfoTable(UDataTable* _Table)
{
	mItemTable = _Table;

	// ������ ���̺� ����ִ� ��� ����ü ����
	FString str;
	TArray<FItemDataTable*> AllItemInfo;
	mItemTable->GetAllRows<FItemDataTable>(str, AllItemInfo);

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

const FItemDataTable* UInventoryManager::GetItemInfo(EITEM_ID _ItemID)
{
	// ItemID �� �����ص� RowName �� ã�Ƽ�
	FName RowName = mRownameMap.FindRef(_ItemID);

	// ������ ���̺��� �˻��� �Ѵ�.
	FItemDataTable* ItemInfo = mItemTable->FindRow<FItemDataTable>(RowName, nullptr);

	return ItemInfo;
}
