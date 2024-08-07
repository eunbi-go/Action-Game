// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterHpWidget.h"
#include "../Player/CharacterStatComponent.h"
#include "WidgetController/MainWidgetController.h"
#include "../Monster/Monster.h"

void UMonsterHpWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mHpBar = Cast<UProgressBar>(GetWidgetFromName(FName(TEXT("HpBar"))));
	mHpBar->SetPercent(1.f);
}

void UMonsterHpWidget::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);

	mHpBar->SetPercent(FMath::FInterpTo(mHpBar->Percent, UKismetMathLibrary::SafeDivide(mHp, mMaxHp), _DeltaTime, 5.f));

	if (mHp <= 0)
	{
		mMonster->Death();
	}
}

void UMonsterHpWidget::SetNewHp(float newHp)
{
	//PrintViewport(3.f, FColor::White, FString("UMonsterHpWidget::SetNewHp"));
	if (IsValid(this))
		mHp = newHp;

}

void UMonsterHpWidget::SetNewMaxHp(float newMaxHp)
{
	if (IsValid(this))
		mMaxHp = newMaxHp;
}