// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "Components/ActorComponent.h"
#include "CharacterStatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnHpIsZero);
DECLARE_MULTICAST_DELEGATE(FOnHpChange);
DECLARE_MULTICAST_DELEGATE(FOnMpChange);



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AG_API UCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterStatComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	void SetDamage(float _damage);
	void SetHp(float _hp);
	void SetMp(float _mp);

	FOnHpIsZero	mHpDecrease;
	FOnHpChange mHpChange;
	FOnMpChange mMpChange;

public:
	float GetAttack() { return mCurrentData->attackPoint; }
	float GetHpRatio();
	float GetHp() { return mCurrentHp; }
	float GetMpRatio();
	float GetMp() { return mCurrentMp; }

private:
	struct FPlayerTableInfo* mCurrentData;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	float	mCurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, meta = (AllowPrivateAccess = true))
	float	mCurrentMp;
};
