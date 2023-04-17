// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "Components/TileView.h"
#include "ItemData.h"
#include "../Manager/InventoryManager.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mTileView = Cast<UTileView>(GetWidgetFromName(FName(TEXT("TileView"))));

	// test.
	UItemData* item = NewObject<UItemData>();

	item->SetIconPath(TEXT("Texture2D'/Game/Viking_RPG_UI_5_0/Buttons/White_buttons/Flat_Icon_18.Flat_Icon_18'"));
	item->SetDescription(TEXT("Mega Potion"));
	item->SetItemCount(5);

	mTileView->AddItem(item);
}

void UInventoryWidget::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);
}

void UInventoryWidget::AddItemByKey(EITEM_ID _id)
{
	UItemData* item = NewObject<UItemData>();

	const FItemDataTable* table = UInventoryManager::GetInst(GetWorld())->GetItemInfo(_id);

	item->SetIconPath(table->iconPath);
	item->SetDescription(table->description);
	item->SetItemCount(1);

	mTileView->AddItem(item);
}
