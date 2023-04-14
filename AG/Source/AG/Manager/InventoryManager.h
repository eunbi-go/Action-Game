// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "UObject/NoExportTypes.h"
#include "InventoryManager.generated.h"

/**
 * 
 */
UCLASS()
class AG_API UInventoryManager : public UObject
{
	GENERATED_BODY()

public:
	UInventoryManager();
	~UInventoryManager();


public:
	static UInventoryManager* GetInst(UWorld* _World);

	UFUNCTION(BlueprintCallable)
	static UInventoryManager* GetInst(UGameInstance* _GameInst);


public:
	UFUNCTION(BlueprintCallable)
	void InventoryOnOff(bool _Show);

	bool IsInventoryOn();
	void SetItemInfoTable(UDataTable* _Table);
	const FItemDataTable* GetItemInfo(EITEM_ID _ItemID);



private:
	static	UWorld* mWorld;

public:
	UDataTable*				mItemTable;
	TMap<EITEM_ID, FName>	mRownameMap;

	int32					mPreTime; // 이전 시간
};
