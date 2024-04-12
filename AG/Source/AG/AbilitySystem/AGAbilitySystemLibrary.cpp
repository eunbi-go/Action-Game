// Fill out your copyright notice in the Description page of Project Settings.


#include "AGAbilitySystemLibrary.h"
#include "../Widget/HUD/AGHUD.h"
#include "../Player/ValkyriePlayerState.h"
#include "../Widget/AGWidgetController.h"

UMainWidgetController* UAGAbilitySystemLibrary::GetMainWidgetController(const UObject* WorldContextObject)
{
	/**
	* Widget Controller�� �����ϴµ� �ʿ��� ��ҵ�
	*/
	APlayerController* pc = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	check(pc);
	
	AAGHUD* hud = Cast<AAGHUD>(pc->GetHUD());
	check(hud);
	
	AValkyriePlayerState* ps = pc->GetPlayerState<AValkyriePlayerState>();
	check(ps);

	UAbilitySystemComponent* asc = ps->GetAbilitySystemComponent();
	UAttributeSet* as = ps->GetAttributeSet();

	const FWidgetControllerParams widgetControllerParams(pc, ps, asc, as);
	return hud->GetMainWidgetController(widgetControllerParams);
}

UAttributeWidgetController* UAGAbilitySystemLibrary::GetAttributeWidgetController(const UObject* WorldContextObject)
{
	APlayerController* pc = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	check(pc);

	AAGHUD* hud = Cast<AAGHUD>(pc->GetHUD());
	check(hud);

	AValkyriePlayerState* ps = pc->GetPlayerState<AValkyriePlayerState>();
	check(ps);

	UAbilitySystemComponent* asc = ps->GetAbilitySystemComponent();
	UAttributeSet* as = ps->GetAttributeSet();

	const FWidgetControllerParams widgetControllerParams(pc, ps, asc, as);
	return hud->GetAttributeWidgetController(widgetControllerParams);
}
