// Fill out your copyright notice in the Description page of Project Settings.


#include "AGAttributeValueButtonRow.h"
#include "AttributeValueRow.h"
#include "../Button/PlusButton.h"
#include "../Button/AGButton.h"
#include "../HUD/AGHUD.h"
#include "../../Player/ValkyriePlayerState.h"
#include "../../Actor/AGEffectActor.h"
#include "../../AbilitySystem/AGAbilitySystemLibrary.h"
#include "../WidgetController/AttributeWidgetController.h"

void UAGAttributeValueButtonRow::NativeConstruct()
{
	Super::NativeConstruct();

	mRow = Cast<UAttributeValueRow>(GetWidgetFromName(TEXT("UI_AttributeValueRow")));
	mPlusButton = Cast<UPlusButton>(GetWidgetFromName(TEXT("UI_PlusButton")));

	mPlusButton->mButton->GetButton()->OnClicked.AddDynamic(this, &UAGAttributeValueButtonRow::PlusButtonClicked);
}

void UAGAttributeValueButtonRow::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UAGAttributeValueButtonRow::PlusButtonClicked()
{
	//PrintViewport(3.f, FColor::Green, FString("UAGAttributeValueButtonRow::PlusButtonClicked()"));

	// Attribute를 증가시켜야 함
	APlayerController* pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	check(pc);

	AAGHUD* hud = Cast<AAGHUD>(pc->GetHUD());
	check(hud);

	AValkyriePlayerState* ps = pc->GetPlayerState<AValkyriePlayerState>();
	check(ps);

	UAbilitySystemComponent* asc = ps->GetAbilitySystemComponent();


	FGameplayEffectContextHandle ecHandle = asc->MakeEffectContext();
	ecHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle esHandle = asc->MakeOutgoingSpec(mEffect, 1.f, ecHandle);
	const FActiveGameplayEffectHandle agpeHandle = asc->ApplyGameplayEffectSpecToSelf(*esHandle.Data.Get());
	
}