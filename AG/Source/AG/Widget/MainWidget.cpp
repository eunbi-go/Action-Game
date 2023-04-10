// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"

#include "../Player/CharacterStatComponent.h"


void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mPlayerInfo = Cast<UPlayerInfoWidget>(GetWidgetFromName(FName(TEXT("PlayerInfo"))));
}

void UMainWidget::NativeTick(const FGeometry& _geo, float _deltaTime)
{
	Super::NativeTick(_geo, _deltaTime);
}

void UMainWidget::SetCharacterStat(UCharacterStatComponent* _characterStat)
{
	if (_characterStat == nullptr)
		return;

	mCurrentStat = _characterStat;
	mCurrentStat->mHpChange.AddUObject(this, &UMainWidget::UpdateHp);
	UpdateHp();

}

void UMainWidget::UpdateHp()
{
	PrintViewport(3.f, FColor::Blue, TEXT("UpdateHp"));
	mPlayerInfo->SetHpRatio(mCurrentStat->GetHpRatio());
}

void UMainWidget::SetHp(float _ratio)
{
	mPlayerInfo->SetHp(_ratio);
}

void UMainWidget::SetMp(float _ratio)
{
	mPlayerInfo->SetMp(_ratio);
}
