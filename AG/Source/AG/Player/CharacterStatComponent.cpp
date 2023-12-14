// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatComponent.h"

#include "../AGGameInstance.h"
#include "../AGGameModeBase.h"
#include "../Widget/MainWidget.h"
#include "../AGSaveGame.h"

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

void UCharacterStatComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	AAGGameModeBase* gameMode = GetWorld()->GetAuthGameMode<AAGGameModeBase>();
	
	FPlayerInfo playerInfo;
	playerInfo.name = mCurrentData->name;
	playerInfo.attackPoint = mCurrentData->attackPoint;
	playerInfo.defensePoint = mCurrentData->defensePoint;
	playerInfo.hp = mCurrentHp;
	playerInfo.maxHp = mCurrentData->maxHp;
	playerInfo.mp = mCurrentMp;
	playerInfo.maxMp = mCurrentData->maxMp;
	playerInfo.level = mCurrentData->level;
	playerInfo.exp = mCurrentData->exp;
	playerInfo.gold = mCurrentData->gold;
	playerInfo.movingWalkSpeed = mCurrentData->movingWalkSpeed;
	playerInfo.movingRunSpeed = mCurrentData->movingRunSpeed;
	playerInfo.movingDashSpeed = mCurrentData->movingDashSpeed;
	playerInfo.attackDistance = mCurrentData->attackDistance;


	gameMode->GetSaveGame()->mPlayerInfo = playerInfo;

	//PrintViewport(10.f, FColor::Blue, TEXT("UCharacterStatComponent::EndPlay"));
}


void UCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	AAGGameModeBase* gameMode = GetWorld()->GetAuthGameMode<AAGGameModeBase>();

	UAGGameInstance* gmaeInstance = Cast<UAGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (IsValid(gmaeInstance))
	{
		mCurrentData = gmaeInstance->FindPlayerTables(TEXT("Player"));

		mCurrentHp = mCurrentData->maxHp;
		SetHp(mCurrentData->maxHp);

		mCurrentMp = mCurrentData->maxMp;
		SetMp(mCurrentData->maxMp);

		mCurrentCoin = mCurrentData->gold;
		SetCoin(mCurrentCoin);
	}

}

void UCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCharacterStatComponent::SetDamage(float _damage)
{
	mCurrentHp = FMath::Clamp<float>(mCurrentHp - _damage, 0.0f, mCurrentData->maxHp);
	mCurrentData->hp = mCurrentHp;

	//if (mCurrentHp <= 0.0f)
	//	mHpZero.Broadcast();
}

void UCharacterStatComponent::SetHp(float _hp)
{
	mCurrentHp = _hp;
	mCurrentData->hp = mCurrentHp;
	mHpChange.Broadcast();

	//PrintViewport(3.f, FColor::Red, FString::Printf(TEXT("mCurrentHp: %f, max: %f, ratio: %f"), mCurrentHp, mCurrentData->maxHp, GetHpRatio()));
	
	if (mCurrentHp < 0.f)
	{
		//mCurrentHp = 0.f;
		//mHpZero.Broadcast();
	}
}

void UCharacterStatComponent::SetMp(float _mp)
{
	mCurrentMp = _mp;
	mCurrentData->mp = mCurrentMp;

	mMpChange.Broadcast();
}

void UCharacterStatComponent::SetCoin(int32 _coin)
{
	mCurrentCoin = _coin;

	mCurrentData->gold = mCurrentCoin;

	mCoinChange.Broadcast();
}

float UCharacterStatComponent::GetHpRatio()
{
	return mCurrentData->maxHp < 0.0f ? 0.0f : (mCurrentHp / mCurrentData->maxHp);
}

float UCharacterStatComponent::GetMpRatio()
{
	return mCurrentData->maxMp < 0.0f ? 0.0f : (mCurrentMp / mCurrentData->maxMp);
}

