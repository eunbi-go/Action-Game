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

	// 인벤토리를 처음으로 요청한 순간
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

	// 인벤토리를 처음으로 요청한 순간
	if (nullptr == gameInst->mInventory)
	{
		gameInst->mInventory = NewObject<UInventoryManager>();
		gameInst->mInventory->AddToRoot();
	}

	return gameInst->mInventory;
}

void UInventoryManager::InventoryOnOff(bool _Show)
{
	// 이전에 호출한 뒤에 일정 시간이 지났는지 체크
	int32 curTime = 0;
	float curPartial = 0.f;
	UGameplayStatics::GetAccurateRealTime(curTime, curPartial);
	// 시간차이가 1초를 넘어서지 못하면 반환
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

	// 인벤토리 온
	if (_Show)
	{
		inventoryWidget->SetVisibility(ESlateVisibility::Visible);

		APlayerController* Controller = mWorld->GetFirstPlayerController();
		
		FInputModeGameAndUI uionly;
		//FInputModeUIOnly uionly;
		Controller->SetInputMode(uionly);
		Controller->bShowMouseCursor = true;
	}

	// 인벤토리 오프
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

	// 데이터 테이블에 들어있는 모든 구조체 정보
	FString str;
	TArray<FItemDataTable2*> AllItemInfo;
	mItemTable->GetAllRows<FItemDataTable2>(str, AllItemInfo);

	// 데이터 테이블에 들어있는 모든 행 이름
	TArray<FName> AllRowname;
	AllRowname = mItemTable->GetRowNames();

	// 테이블정보를 다 받아와서 전체 반복문을 돈다.
	// ItemID 를 키값으로, 해당 아이템의 RowName 을 데이터로 연결한다.
	for (int i = 0; i < AllItemInfo.Num(); ++i)
	{
		mRownameMap.Add(AllItemInfo[i]->id, AllRowname[i]);
	}
}

const FItemDataTable2* UInventoryManager::GetItemInfo(EITEM_ID _ItemID)
{
	// ItemID 에 연결해둔 RowName 을 찾아서
	FName RowName = mRownameMap.FindRef(_ItemID);

	// 데이터 테이블에서 검색을 한다.
	FItemDataTable2* ItemInfo = mItemTable->FindRow<FItemDataTable2>(RowName, nullptr);

	return ItemInfo;
}
