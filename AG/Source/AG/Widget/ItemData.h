// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "UObject/NoExportTypes.h"
#include "ItemData.generated.h"

/**
 * 
 */
UCLASS()
class AG_API UItemData : public UObject
{
	GENERATED_BODY()

public:
	UItemData();
	~UItemData();


public:
	void SetIconPath(const FString& _IconPath) { mIconPath = _IconPath; }
	const FString& GetIconPath() { return mIconPath; }

	void SetDescription(const FString& _Desc) { mDescription = _Desc; }
	const FString& GetDescription() { return mDescription; }

	void SetItemCount(int32 _Count) { mItemCount = _Count; }
	int32 GetItemCount() { return mItemCount; }

	void SetItemIndex(int32 _index) { mItemIndex = _index; }
	int32 GetItemIndex() { return mItemIndex; }

	EITEM_ID GetItemId() { return mId; }
	void SetItemId(EITEM_ID _id) { mId = _id; }

private:
	FString		mIconPath;		// ������ �̹��� ���
	FString		mDescription;	// ������ ����
	int32		mItemCount;	// ������ ����
	int32		mItemIndex;	// ������ �ε��� (�����Կ��� ���)
	EITEM_ID	mId;
};
