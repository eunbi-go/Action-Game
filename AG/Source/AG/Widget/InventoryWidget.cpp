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
	// _id번 아이템의 정보를 가져온다.
	//---------------------------------
	const FItemDataTable* table = UInventoryManager::GetInst(GetWorld())->GetItemInfo(_id);

	//---------------------------------
	// Map에 _id번 아이템이 있으면 개수만 변경시킨다.
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
	// Map에 _id번 아이템이 없으면 
	// ItemData 를 생성해서 인벤토리와 mItemMap에 추가한다.
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
	// _id번 아이템이 있으면 개수를 변경한다.
	//		- 만약 0개가 되면 아이템을 삭제한다.
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

// item: 클릭된 아이템.


void UInventoryWidget::Clicked(UObject* item)
{

	UItemData* itemData = Cast<UItemData>(item);

	//---------------------------------
	// 선택된 아이템 (item)의 데이터를 퀵슬롯에 전달한다.
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
