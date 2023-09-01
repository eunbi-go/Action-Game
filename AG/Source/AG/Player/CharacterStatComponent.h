// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "Components/ActorComponent.h"
#include "CharacterStatComponent.generated.h"






UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AG_API UCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE(mHpDecrease);
	DECLARE_MULTICAST_DELEGATE(FOnHpChange);
	DECLARE_MULTICAST_DELEGATE(FOnMpChange);
	DECLARE_MULTICAST_DELEGATE(FOnCoinChange);

public:	
	UCharacterStatComponent();

	mHpDecrease	mHpZero;
	FOnHpChange mHpChange;
	FOnMpChange mMpChange;
	FOnCoinChange	mCoinChange;


protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

public:	
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	void SetDamage(float _damage);
	void SetHp(float _hp);
	void SetMp(float _mp);
	void SetCoin(int32 _coin);
	void InitCoin(int32 _coin);


public:
	const FPlayerTableInfo& GetCurrentInfo() 
	{ 
		return *mCurrentData; 
	}

	FPlayerTableInfo& GetInfo()
	{
		return *mCurrentData; 
	}

	float GetAttack() 
	{
		return mCurrentData->attackPoint; 
	}

	float GetHpRatio();
	
	float GetHp() 
	{
		return mCurrentHp; 
	}

	float GetMpRatio();
	
	float GetMp() 
	{
		return mCurrentMp; 
	}

	float GetCoin()
	{
		return mCurrentCoin;
	}

private:
	struct FPlayerTableInfo* mCurrentData;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	float	mCurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	float	mCurrentMp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	float	mCurrentCoin;
};
