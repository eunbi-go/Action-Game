

#include "InventoryItem.h"
#include "ItemData.h"

void UInventoryItem::NativeConstruct()
{
	Super::NativeConstruct();

	mIconImage = Cast<UImage>(GetWidgetFromName(FName(TEXT("Icon"))));
	mCountTxt = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("CountText"))));
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
		int Count = data->GetItemCount();

		// �����Ϳ� ����ִ� IconPath �� �̿��ؼ� �ش� �ؽ��ĸ� �ε� ��, 
		// Image ������ �ش� �ؽ��ĸ� �������ش�.
		UTexture2D* pTex2D = LoadObject<UTexture2D>(nullptr, *IconPath);
		if (IsValid(pTex2D))
		{
			mIconImage->SetBrushFromTexture(pTex2D);
		}

		//// �����Ϳ� ����ִ� ������ �̸��� ItemName �� ����� �� �ؽ�Ʈ���� ������ ����
		//m_ItemNameTxt->SetText(FText::FromString(ItemName));

		// �����Ϳ� ����ִ� ������ ������ ���ڿ��� �ٲپ, ItemCount �ؽ�Ʈ���� ������ ����		
		mCountTxt->SetText(FText::FromString(FString::Printf(TEXT("%d"), Count)));
	}
}