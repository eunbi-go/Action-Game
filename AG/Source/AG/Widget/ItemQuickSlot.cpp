// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemQuickSlot.h"
#include "ItemData.h"
#include "../Manager/InventoryManager.h"

void UItemQuickSlot::NativeConstruct()
{
	Super::NativeConstruct();

	mListView = Cast<UListView>(GetWidgetFromName(FName(TEXT("ListView"))));
}

void UItemQuickSlot::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);
}

void UItemQuickSlot::AddItemToQuickSlot(UItemData* _itemData)
{
	const FItemDataTable* table = UInventoryManager::GetInst(GetWorld())->GetItemInfo(_itemData->GetItemId());

	UItemData* item = NewObject<UItemData>();

	item->SetIconPath(table->iconPath);
	item->SetDescription(table->description);
	item->SetItemCount(_itemData->GetItemCount());
	item->SetItemId(_itemData->GetItemId());

	mListView->AddItem(item);
}
