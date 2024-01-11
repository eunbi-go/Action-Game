// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemQuickSlot.h"
#include "ItemData.h"
#include "../Manager/InventoryManager.h"
#include "../AGGameInstance.h"
#include "../AGGameModeBase.h"
#include "../Widget/MainWidget.h"
#include "../Widget/InventoryWidget.h"
#include "../Player/PlayerCharacter.h"


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

	/*AAGGameModeBase* gameMode = Cast<AAGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (nullptr == gameMode)
		return;


	UMainWidget* mainHUD = gameMode->GetMainWidget();
	UInventoryWidget* inveotyrWidget = mainHUD->GetInventoryWidget();

	TArray<UObject*> inventoryItemArray = inveotyrWidget->GetTileView()->GetListItems();
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
	}*/

	
}

void UItemQuickSlot::UseItem(int32 _index, APlayerCharacter* userObject)
{
	TArray<UObject*> quickSlotItemArray = mListView->GetListItems();
	int32 num = quickSlotItemArray.Num();

	int32 itemCount = Cast<UItemData>(quickSlotItemArray[_index - 1])->GetItemCount();

	EITEM_ID id = Cast<UItemData>(quickSlotItemArray[_index - 1])->GetItemId();

	if (itemCount - 1 == 0)
	{
		//mListView->RemoveItem(quickSlotItemArray[_index - 1]);

		// 나머지 인덱스 앞으로 땡겨주기.
		Cast<UItemData>(quickSlotItemArray[_index - 1])->SetIconPath(TEXT("Texture2D'/Game/Viking_RPG_UI_5_0/back.back'"));
		Cast<UItemData>(quickSlotItemArray[_index - 1])->SetItemId(EITEM_ID::END);
		Cast<UItemData>(quickSlotItemArray[_index - 1])->SetItemCount(0);
	}
	else
	{
		Cast<UItemData>(quickSlotItemArray[_index - 1])->SetItemCount(itemCount - 1);
	}

	mListView->RegenerateAllEntries();

	mUseItems.Broadcast(id, userObject);
}

void UItemQuickSlot::AddItemToQuickSlot(UItemData* _itemData)
{
	const FItemDataTable* table = UInventoryManager::GetInst(GetWorld())->GetItemInfo(_itemData->GetItemId());

	TArray<UObject*> quickSlotItemArray = mListView->GetListItems();

	for (int32 i = 0; i < mQuickSlotSize; ++i)
	{
		UItemData* item = Cast<UItemData>(quickSlotItemArray[i]);
		if (item->GetItemId() == EITEM_ID::END)
		{
			item->SetIconPath(table->iconPath);
			item->SetDescription(table->description);
			item->SetItemCount(_itemData->GetItemCount());
			item->SetItemId(_itemData->GetItemId());
			break;
		}
	}

	mListView->RegenerateAllEntries();
}
