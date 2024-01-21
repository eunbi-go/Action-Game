// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "ItemData.h"
#include "../Manager/InventoryManager.h"
#include "InventoryItem.h"
#include "ItemQuickSlot.h"
#include "../AGGameInstance.h"
#include "../AGGameModeBase.h"
#include "MainWidget.h"
#include "../Player/PlayerCharacter.h"
#include "HUD/AGHUD.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mTileView = Cast<UTileView>(GetWidgetFromName(FName(TEXT("TileView"))));

	// test.
	//UItemData* item = NewObject<UItemData>();

	//item->SetIconPath(TEXT("Texture2D'/Game/Viking_RPG_UI_5_0/Buttons/White_buttons/Flat_Icon_18.Flat_Icon_18'"));
	//item->SetDescription(TEXT("Mega Potion"));
	//item->SetItemCount(5);

	//mTileView->AddItem(item);
}

void UInventoryWidget::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);
}

void UInventoryWidget::AddItemByKey(EITEM_ID _id)
{
	//---------------------------------
	// _id�� �������� ������ �����´�.
	//---------------------------------
	const FItemDataTable* table = UInventoryManager::GetInst(GetWorld())->GetItemInfo(_id);

	//---------------------------------
	// Map�� _id�� �������� ������ ������ �����Ų��.
	//---------------------------------
	if (mItemMap.Contains(_id))
	{
		TArray<UObject*> item = mTileView->GetListItems();
		
		int32 count = item.Num();
		int32 index = -1;

		for (int32 i = 0; i < count; ++i)
		{
			if (Cast<UItemData>(item[i])->GetItemId() == _id)
			{
				index = i;
				break;
			}
		}

		UItemData* itemData = Cast<UItemData>(mTileView->GetItemAt(index));
		itemData->SetItemCount(itemData->GetItemCount() + 1);
		mTileView->RegenerateAllEntries();
	}
	//---------------------------------
	// Map�� _id�� �������� ������ 
	// ItemData �� �����ؼ� �κ��丮�� mItemMap�� �߰��Ѵ�.
	//---------------------------------
	else
	{
		UItemData* item = NewObject<UItemData>();

		item->SetIconPath(table->iconPath);
		item->SetDescription(table->description);
		item->SetItemCount(1);
		item->SetItemId(_id);
		item->SetItemEffect(table->effect);

		mItemMap.Add(_id, item);
		mTileView->AddItem(item);
	}

	
}

void UInventoryWidget::UseItem(EITEM_ID _id, ACharacter* userCharacter)
{
	TArray<UObject*> itemArray = mTileView->GetListItems();
	int32 num = itemArray.Num();

	//---------------------------------
	// _id�� �������� ������ ������ �����Ѵ�.
	//		- ���� 0���� �Ǹ� �������� �����Ѵ�.
	//---------------------------------
	for (int32 i = 0; i < num; ++i)
	{
		UItemData* item = Cast<UItemData>(itemArray[i]);

		if (item->GetItemId() == _id)
		{
			int32 itemCount = item->GetItemCount();
			if (itemCount - 1 == 0)
			{
				mTileView->RemoveItem(item);
				mItemMap.Remove(_id);
			}
			else
				item->SetItemCount(itemCount - 1);

			mTileView->RegenerateAllEntries();
			break;
		}
	}
}

// item: Ŭ���� ������.


void UInventoryWidget::Clicked(UObject* item)
{

	UItemData* itemData = Cast<UItemData>(item);

	//---------------------------------
	// ���õ� ������ (item)�� �����͸� �����Կ� �����Ѵ�.
	//---------------------------------
	if (IsValid(itemData))
	{
		AAGGameModeBase* gameMode = Cast<AAGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (nullptr == gameMode)
			return;
		AAGHUD* hud = Cast<AAGHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		if (!IsValid(hud))
			return;
		UMainWidget* mainWidget = hud->mMainWidget;
		if (!IsValid(mainWidget))
			return;
		UItemQuickSlot* quickSlot = mainWidget->GetItemQuickSlot();

		quickSlot->AddItemToQuickSlot(itemData);
	}
}
