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
	const FItemDataTable2* table = UInventoryManager::GetInst(GetWorld())->GetItemInfo(_id);

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
	// 아이템 사용.
	//userCharacter->UseItem(_id);

	// 인벤토리 업데이트.
	TArray<UObject*> itemArray = mTileView->GetListItems();
	int32 num = itemArray.Num();

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
	//PrintViewport(20.f, FColor::Yellow, TEXT("click"));

	UItemData* itemData = Cast<UItemData>(item);

	if (IsValid(itemData))
	{
		AAGGameModeBase* GameMode = Cast<AAGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

		// 알아낸 게임모드가 AAGGameModeBase 가 아니라면 캐스팅 실패 == 현재 월드가 메인 레벨이 아니라는 뜻
		if (nullptr == GameMode)
			return;


		// 현재 게임모드가 AAGGameModeBase 가 맞다면, MainHUD 에 접근해서 InventoryWiget 의 Visible 여부를 확인한다.
		AAGHUD* hud = Cast<AAGHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		if (!IsValid(hud))
			return;
		UMainWidget* mainWidget = hud->mMainWidget;
		if (!IsValid(mainWidget))
			return;
		UItemQuickSlot* quickSlot = mainWidget->GetItemQuickSlot();

		EITEM_ID id = itemData->GetItemId();
		quickSlot->AddItemToQuickSlot(itemData);
	}
}
