

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
		// �Էµ� �����Ϳ� ����ִ� ������ InventoryItem �� �ʱ�ȭ �ȴ�.	
		const FString& IconPath = data->GetIconPath();
		const FString& ItemName = data->GetDescription();
		int32 Count = data->GetItemCount();
		int32 index = data->GetItemIndex();

		// �����Ϳ� ����ִ� IconPath �� �̿��ؼ� �ش� �ؽ��ĸ� �ε� ��, 
		// Image ������ �ش� �ؽ��ĸ� �������ش�.
		UTexture2D* pTex2D = LoadObject<UTexture2D>(nullptr, *IconPath);
		if (IsValid(pTex2D))
		{
			mIconImage->SetBrushFromTexture(pTex2D);
		}

		//// �����Ϳ� ����ִ� ������ �̸��� ItemName �� ����� �� �ؽ�Ʈ��� ������ ����
		//m_ItemNameTxt->SetText(FText::FromString(ItemName));

		// �����Ϳ� ����ִ� ������ ������ ���ڿ��� �ٲپ, ItemCount �ؽ�Ʈ��� ������ ����		
		mCountTxt->SetText(FText::FromString(FString::Printf(TEXT("%d"), Count)));
		if (index != 0)
			mIndexTxt->SetText(FText::FromString(FString::Printf(TEXT("%d"), index)));
	}
}
