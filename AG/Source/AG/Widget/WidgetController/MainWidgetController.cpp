// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidgetController.h"
#include "../../AbilitySystem/AGAttributeSet.h"

void UMainWidgetController::BroadcastInitValues()
{
	const UAGAttributeSet* as = CastChecked<UAGAttributeSet>(mAttributeSet);
	mOnHpChange.Broadcast(as->GetmHp());
	mOnMaxHpChange.Broadcast(as->GetmMaxHp());

	//mAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(as->GetmHpAttribute());
}
