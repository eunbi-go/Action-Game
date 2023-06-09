// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BasicActor.h"
#include "WeaponActor.generated.h"

UCLASS()
class AG_API AWeaponActor : public ABasicActor
{
	GENERATED_BODY()
	
public:
	AWeaponActor();

protected:
	virtual void BeginPlay() override;

public:
	void IsGaugeOn(bool _value) 
	{ 
		mGauge->SetVisibility(_value);
	}

	void SetTrailOnOff(bool _value) 
	{ 
		mTrail->SetVisibility(_value);
	}



protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	UNiagaraComponent* mTrail;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	UNiagaraComponent* mGauge;
};























