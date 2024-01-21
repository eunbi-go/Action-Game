// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemQuickSlot.h"
#include "ItemData.h"
#include "../Manager/InventoryManager.h"
#include "../AGGameInstance.h"
#include "../AGGameModeBase.h"
#include "../Widget/MainWidget.h"
#include "../Widget/InventoryWidget.h"
#include "../Player/PlayerCharacter.h"
#include "HUD/AGHUD.h"

void UItemQuickSlot::NativeConstruct()
{
	Super::NativeConstruct();

	mListView = Cast<UListView>(GetWidgetFromName(FName(TEXT("ListView"))));

	mQuickSlotSize = 5;


	for (int32 i = 0; i < mQuickSlotSize; ++i)
	{
		UItemData* item = NewObject<UItemData>();

		item->SetIconPath(TEXT("Texture2D'/Game/Viking_RPG_UI_5_0/back.back'"));
		item->SetItemId(EITEM_ID::END);
		item->SetItemIndex(i+1);

		mListView->AddItem(item);
	}
}

void UItemQuickSlot::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);

	AAGGameModeBase* gameMode = Cast<AAGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (nullptr == gameMode)
		return;

	AAGHUD* hud = Cast<AAGHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (!IsValid(hud))
		return;
	UMainWidget* mainWidget = hud->mMainWidget;
	if (!IsValid(hud))
		return;
	UInventoryWidget* inventoryWidget = mainWidget->GetInventoryWidget();



	TArray<UObject*> inventoryItemArray = inventoryWidget->GetTileView()->GetListItems();
	TArray<UObject*> quickSlotItemArray = mListView->GetListItems();

	int32 num = quickSlotItemArray.Num();

	for (int32 i = 0; i < num; ++i)
	{
		UItemData* quickSlotItem = Cast<UItemData>(quickSlotItemArray[i]);
		EITEM_ID quickSlotItemId = quickSlotItem->GetItemId();

		int32 inventoryNum = inventoryItemArray.Num();

		for (int32 j = 0; j < inventoryNum; ++j)
		{
			UItemData* inventoryItem = Cast<UItemData>(inventoryItemArray[j]);
			if (quickSlotItemId == inventoryItem->GetItemId())
			{
				quickSlotItem->SetItemCount(inventoryItem->GetItemCount());
				mListView->RegenerateAllEntries();
				break;
			}
		}
	}

	
}

void UItemQuickSlot::UseItem(int32 _index, ACharacter* userObject)
{
	TArray<UObject*> quickSlotItemArray = mListView->GetListItems();
	int32 num = quickSlotItemArray.Num();

	int32 itemCount = Cast<UItemData>(quickSlotItemArray[_index - 1])->GetItemCount();
	EITEM_ID id = Cast<UItemData>(quickSlotItemArray[_index - 1])->GetItemId();
	
	//---------------------------------
	// 퀵슬롯에 아이템이 없으면 return
	//---------------------------------
	if (itemCount == 0)
		return;

	//---------------------------------
	// 퀵슬롯에 아이템이 하나 남았으면 빈 칸으로 변경
	//---------------------------------
	if (itemCount - 1 == 0)
	{
		Cast<UItemData>(quickSlotItemArray[_index - 1])->SetIconPath(TEXT("Texture2D'/Game/Viking_RPG_UI_5_0/back.back'"));
		Cast<UItemData>(quickSlotItemArray[_index - 1])->SetItemId(EITEM_ID::END);
		Cast<UItemData>(quickSlotItemArray[_index - 1])->SetItemCount(0);
	}
	//---------------------------------
	// 퀵슬롯에 아이템이 더 남았으면 개수만 변경
	//---------------------------------
	else
	{
		Cast<UItemData>(quickSlotItemArray[_index - 1])->SetItemCount(itemCount - 1);
	}

	mListView->RegenerateAllEntries();


	//---------------------------------
	// 사용하려는 아이템의 효과를 적용하고, 
	// 인벤토리 위젯에서도 정보를 변경시킨다.
	//---------------------------------
	Cast<UItemData>(quickSlotItemArray[_index - 1])->ApplyEffect(userObject);

	AAGGameModeBase* gameMode = Cast<AAGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (nullptr == gameMode)
		return;
	AAGHUD* hud = Cast<AAGHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (!IsValid(hud))
		return;
	UMainWidget* mainWidget = hud->mMainWidget;
	if (!IsValid(hud))
		return;

	UInventoryWidget* inventoryWidget = mainWidget->GetInventoryWidget();
	inventoryWidget->UseItem(id, userObject);
}

void UItemQuickSlot::AddItemToQuickSlot(UItemData* _itemData)
{
	const FItemDataTable* table = UInventoryManager::GetInst(GetWorld())->GetItemInfo(_itemData->GetItemId());
	TArray<UObject*> quickSlotItemArray = mListView->GetListItems();

	//---------------------------------
	// 퀵슬롯에 _itemData가 이미 존재하면 개수만 늘린다.
	//---------------------------------
	bool flag = false;
	for (int32 i = 0; i < mQuickSlotSize; ++i)
	{
		UItemData* item = Cast<UItemData>(quickSlotItemArray[i]);
		if (item->GetItemId() == table->id)
		{
			item->SetItemCount(_itemData->GetItemCount()+1);
			flag = true;
			break;
		}
	}

	mListView->RegenerateAllEntries();

	if (flag)
		return;

	//---------------------------------
	// 퀵슬롯에 _itemData가 없으면 비어있는 칸들 중 제일 앞 칸에 넣는다.
	// 아이템 정보도 같이 넣어준다.
	//---------------------------------
	for (int32 i = 0; i < mQuickSlotSize; ++i)
	{
		UItemData* item = Cast<UItemData>(quickSlotItemArray[i]);
		if (item->GetItemId() == EITEM_ID::END)
		{
			item->SetIconPath(table->iconPath);
			item->SetDescription(table->description);
			item->SetItemCount(_itemData->GetItemCount() + 1);
			item->SetItemId(_itemData->GetItemId());
			item->SetItemEffect(_itemData->mEffect);
			break;
		}
	}

	mListView->RegenerateAllEntries();
}
