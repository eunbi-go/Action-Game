// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatComponent.h"

#include "../AGGameInstance.h"
#include "../AGGameModeBase.h"
#include "../Widget/MainWidget.h"


UCharacterStatComponent::UCharacterStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	mCurrentData = nullptr;
}


void UCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();


	UAGGameInstance* gmaeInstance = Cast<UAGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	mCurrentData = gmaeInstance->FindPlayerTables(TEXT("Player"));
	mCurrentHp = mCurrentData->maxHp;
	SetHp(mCurrentData->maxHp);
}

void UCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCharacterStatComponent::SetDamage(float _damage)
{
	mCurrentHp = FMath::Clamp<float>(mCurrentHp - _damage, 0.0f, mCurrentData->maxHp);

	if (mCurrentHp <= 0.0f)
		mHpDecrease.Broadcast();
}

void UCharacterStatComponent::SetHp(float _hp)
{
	mCurrentHp = _hp;
	mHpChange.Broadcast();

	//PrintViewport(3.f, FColor::Red, FString::Printf(TEXT("mCurrentHp: %f, max: %f, ratio: %f"), mCurrentHp, mCurrentData->maxHp, GetHpRatio()));
	
	if (mCurrentHp < 0.f)
	{
		mCurrentHp = 0.f;
		mHpDecrease.Broadcast();
	}
}

float UCharacterStatComponent::GetHpRatio()
{
	return mCurrentData->maxHp < 0.0f ? 0.0f : (mCurrentHp / mCurrentData->maxHp);
}

