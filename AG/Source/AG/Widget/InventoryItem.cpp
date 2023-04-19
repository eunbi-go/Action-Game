

#include "InventoryItem.h"
#include "ItemData.h"
#include "../Manager/InventoryManager.h"
#include "../Widget/InventoryWidget.h"
#include "../AGGameModeBase.h"
#include "../AGGameInstance.h"
#include "../Widget/MainWidget.h"

void UInventoryItem::NativeConstruct()
{
	Super::NativeConstruct();

	mIconImage = Cast<UImage>(GetWidgetFromName(FName(TEXT("Icon"))));
	mCountTxt = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("CountText"))));
	mIndexTxt = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("IndexText"))));
}

void UInventoryItem::NativeTick(const FGeometry& _geo, float _DT)
{
	Super::NativeTick(_geo, _DT);
}

void UInventoryItem::InitFromData(UObject* _Data)
{
	UItemData* data = Cast<UItemData>(_Data);

	if (IsValid(data))
	{
		// 입력된 데이터에 들어있는 값으로 InventoryItem 이 초기화 된다.	
		const FString& IconPath = data->GetIconPath();
		const FString& ItemName = data->GetDescription();
		int32 Count = data->GetItemCount();
		int32 index = data->GetItemIndex();

		// 데이터에 들어있던 IconPath 를 이용해서 해당 텍스쳐를 로딩 후, 
		// Image 위젯에 해당 텍스쳐를 설정해준다.
		UTexture2D* pTex2D = LoadObject<UTexture2D>(nullptr, *IconPath);
		if (IsValid(pTex2D))
		{
			mIconImage->SetBrushFromTexture(pTex2D);
		}

		//// 데이터에 들어있던 아이템 이름을 ItemName 을 출력해 줄 텍스트블록 위젯에 전달
		//m_ItemNameTxt->SetText(FText::FromString(ItemName));

		// 데이터에 들어있던 아이템 수량을 문자열로 바꾸어서, ItemCount 텍스트블록 위젯에 전달		
		mCountTxt->SetText(FText::FromString(FString::Printf(TEXT("%d"), Count)));
		if (index != 0)
			mIndexTxt->SetText(FText::FromString(FString::Printf(TEXT("%d"), index)));
	}
}
