// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterHpWidget.h"
#include "../Player/CharacterStatComponent.h"

void UMonsterHpWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mHpTargetRatio = 1.f;

	mHpBar = Cast<UProgressBar>(GetWidgetFromName(FName(TEXT("HpBar"))));
	mHpBar->SetPercent(mHpTargetRatio);
}

void UMonsterHpWidget::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);

	if (mHpBar->Percent >= mHpTargetRatio)
		mHpBar->SetPercent(FMath::FInterpTo(mHpBar->Percent, mHpTargetRatio, _DeltaTime, 5.f));
}

void UMonsterHpWidget::SetCharacterStat(UCharacterStatComponent* _characterStat)
{
	if (_characterStat == nullptr)
		return;

	mCurrentStat = _characterStat;
	mCurrentStat->mHpChange.AddUObject(this, &UMonsterHpWidget::UpdateHp);
	UpdateHp();
}

void UMonsterHpWidget::UpdateHp()
{
	SetTargetRatio(mCurrentStat->GetHpRatio());
}