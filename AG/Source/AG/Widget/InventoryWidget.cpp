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
	// ������ ���.
	//userCharacter->UseItem(_id);

	// �κ��丮 ������Ʈ.
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

// item: Ŭ���� ������.
void UInventoryWidget::Clicked(UObject* item)
{
	//PrintViewport(20.f, FColor::Yellow, TEXT("click"));

	UItemData* itemData = Cast<UItemData>(item);

	if (IsValid(itemData))
	{
		AAGGameModeBase* GameMode = Cast<AAGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

		// �˾Ƴ� ���Ӹ�尡 AAGGameModeBase �� �ƴ϶�� ĳ���� ���� == ���� ���尡 ���� ������ �ƴ϶�� ��
		if (nullptr == GameMode)
			return;


		// ���� ���Ӹ�尡 AAGGameModeBase �� �´ٸ�, MainHUD �� �����ؼ� InventoryWiget �� Visible ���θ� Ȯ���Ѵ�.
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
