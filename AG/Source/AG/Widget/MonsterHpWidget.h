// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <Components\ProgressBar.h>

#include "../BasicInfo.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffectTypes.h"
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


	UFUNCTION()
	void SetNewHp(float newHp);
	UFUNCTION()
	void SetNewMaxHp(float newMaxHp);

	void SetMonster(class AMonster* Monster)
	{
		mMonster = Monster;
	}



private:
	UProgressBar*	mHpBar;
	float			mHp;
	float			mMaxHp;

	class AMonster* mMonster;
};
