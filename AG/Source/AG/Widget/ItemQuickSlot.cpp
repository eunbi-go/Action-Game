// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemQuickSlot.h"
#include "ItemData.h"
#include "../Manager/InventoryManager.h"
#include "../AGGameInstance.h"
#include "../AGGameModeBase.h"
#include "../Widget/MainWidget.h"
#include "../Widget/InventoryWidget.h"

void UItemQuickSlot::NativeConstruct()
{
	Super::NativeConstruct();

	mListView = Cast<UListView>(GetWidgetFromName(FName(TEXT("ListView"))));
}

void UItemQuickSlot::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);

	AAGGameModeBase* gameMode = Cast<AAGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

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
	}

	
}

void UItemQuickSlot::AddItemToQuickSlot(UItemData* _itemData)
{
	const FItemDataTable* table = UInventoryManager::GetInst(GetWorld())->GetItemInfo(_itemData->GetItemId());

	UItemData* item = NewObject<UItemData>();

	item->SetIconPath(table->iconPath);
	item->SetDescription(table->description);
	item->SetItemCount(_itemData->GetItemCount());
	item->SetItemId(_itemData->GetItemId());

	if (mListView->GetNumItems() == 0)
		item->SetItemIndex(1);
	else if (mListView->GetNumItems() == 1)
		item->SetItemIndex(2);
	else if (mListView->GetNumItems() == 2)
		item->SetItemIndex(3);
	else if (mListView->GetNumItems() == 3)
		item->SetItemIndex(4);
	else if (mListView->GetNumItems() == 4)
		item->SetItemIndex(5);
	else
		return;


	mListView->AddItem(item);
}
