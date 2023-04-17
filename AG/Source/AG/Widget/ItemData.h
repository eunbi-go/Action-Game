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

	void SetItemCount(int _Count) { mItemCount = _Count; }
	int GetItemCount() { return mItemCount; }

	EITEM_ID GetItemId() { return mId; }
	void SetItemId(EITEM_ID _id) { mId = _id; }

private:
	FString		mIconPath;		// 아이콘 이미지 경로
	FString		mDescription;	// 아이템 설명
	int			mItemCount;	// 아이템 수량
	EITEM_ID	mId;
};
