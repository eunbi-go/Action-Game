// Fill out your copyright notice in the Description page of Project Settings.


#include "ValkyriePlayerState.h"
#include "../AbilitySystem/AGAbilitySystemComponent.h"
#include "../AbilitySystem/AGAttributeSet.h"
#include "Net/UnrealNetwork.h"

AValkyriePlayerState::AValkyriePlayerState()
{
	mAbilitySystemComp = CreateDefaultSubobject<UAGAbilitySystemComponent>("AbilitySystemComp");
	mAbilitySystemComp->SetIsReplicated(true);
	mAbilitySystemComp->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	mAttributeSet = CreateDefaultSubobject<UAGAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f;

	//// init AGAttributes by Data Table
	//FAttributeDefaults attributes;

	//static ConstructorHelpers::FObjectFinder<UDataTable>	valkyrieDataTable(TEXT("DataTable'/Game/Blueprints/DataTable/DT_InitValkyrie.DT_InitValkyrie'"));
	//if (valkyrieDataTable.Succeeded())
	//{
	//	attributes.DefaultStartingTable = valkyrieDataTable.Object;
	//}
	//attributes.Attributes = UAGAttributeSet::StaticClass();

	//mAbilitySystemComp->DefaultStartingData.Add(attributes);
}

void AValkyriePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AValkyriePlayerState, mLevel);
}

UAbilitySystemComponent* AValkyriePlayerState::GetAbilitySystemComponent() const
{
	return mAbilitySystemComp;
}

void AValkyriePlayerState::OnRep_Level(int32 PreLevel)
{
}
