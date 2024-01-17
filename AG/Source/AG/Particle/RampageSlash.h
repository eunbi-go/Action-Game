// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ParticleNiagara.h"
#include "RampageSlash.generated.h"

/**
 * 
 */
UCLASS()
class AG_API ARampageSlash : public AParticleNiagara
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHit, FVector, impactPoint);

public:
	ARampageSlash();

	UFUNCTION()
	void Stop();

	UPROPERTY(BlueprintAssignable)
		FOnHit	mOnHit;

	UFUNCTION()
	void MoveStart();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


public:
	void SetIsEnableMove(bool _value)
	{
		mIsEnableMove = _value; 
	}
	void SetTarget(ACharacter* target)
	{
		mTarget = target;
	}


	UPROPERTY()
		FTimerHandle mTimerHandle2;

protected:
	bool	mIsEnableMove;
	ACharacter* mTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
		TSubclassOf<UCameraShakeBase> mHitShake;

	//FVector targetPosition;
};
