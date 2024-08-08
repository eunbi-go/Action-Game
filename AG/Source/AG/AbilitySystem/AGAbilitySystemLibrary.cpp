// Fill out your copyright notice in the Description page of Project Settings.


#include "AGAbilitySystemLibrary.h"
#include "../Widget/HUD/AGHUD.h"
#include "../Player/ValkyriePlayerState.h"
#include "../Widget/AGWidgetController.h"
#include "../AGGameModeBase.h"

UAbilitySystemComponent* UAGAbilitySystemLibrary::GetAbilitySystemComponent(AActor* Actor)
{
	APlayerController* pc = UGameplayStatics::GetPlayerController(Actor, 0);
	check(pc);

	AAGHUD* hud = Cast<AAGHUD>(pc->GetHUD());
	check(hud);

	AValkyriePlayerState* ps = pc->GetPlayerState<AValkyriePlayerState>();
	check(ps);

	UAbilitySystemComponent* asc = ps->GetAbilitySystemComponent();
	return asc;
}

UMainWidgetController* UAGAbilitySystemLibrary::GetMainWidgetController(const UObject* WorldContextObject)
{
	/**
	* Widget Controller를 구성하는데 필요한 요소들
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

void UAGAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AAGGameModeBase* gm = Cast<AAGGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (gm == nullptr)
		return;
	
	AActor* avatar = ASC->GetAvatarActor();


	UCharacterInfo* characterInfo = gm->mCharacterInfo;
	FCharacterClassDefaultInfo classDefaultInfo = characterInfo->GetClassDefaultInfo(CharacterClass);
	
	
	FGameplayEffectContextHandle primaryAttributeContextHandle = ASC->MakeEffectContext();
	primaryAttributeContextHandle.AddSourceObject(avatar);
	const FGameplayEffectSpecHandle primaryAttributeSpec = ASC->MakeOutgoingSpec(classDefaultInfo.mPrimaryAttribute, Level, primaryAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*primaryAttributeSpec.Data.Get());

	FGameplayEffectContextHandle secondaryAttributeContextHandle = ASC->MakeEffectContext();
	secondaryAttributeContextHandle.AddSourceObject(avatar);
	const FGameplayEffectSpecHandle secondaryAttributeSpec = ASC->MakeOutgoingSpec(characterInfo->mSecondaryAttribute, Level, secondaryAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*secondaryAttributeSpec.Data.Get());

	FGameplayEffectContextHandle vitalAttributeContextHandle = ASC->MakeEffectContext();
	vitalAttributeContextHandle.AddSourceObject(avatar);
	const FGameplayEffectSpecHandle vitalAttributeSpec = ASC->MakeOutgoingSpec(characterInfo->mVitalAttribute, Level, vitalAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*vitalAttributeSpec.Data.Get());
}
