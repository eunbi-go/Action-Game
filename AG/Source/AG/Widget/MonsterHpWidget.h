// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <Components\ProgressBar.h>

#include "../BasicInfo.h"
#include "Blueprint/UserWidget.h"
#include "MonsterHpWidget.generated.h"

/**
 * 
 */
UCLASS()
class AG_API UMonsterHpWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _DeltaTime) override;


public:
	void UpdateHp();


public:
	void SetCharacterStat(class UCharacterStatComponent* _characterStat);
	void SetTargetRatio(float _ratio) { mHpTargetRatio = _ratio; }


private:
	UProgressBar*	mHpBar;
	float			mHpTargetRatio;
	TWeakObjectPtr<class UCharacterStatComponent> mCurrentStat;
};
