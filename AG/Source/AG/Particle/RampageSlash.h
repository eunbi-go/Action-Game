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

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReSpawn, class ARampageSlash*, preParticle);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHit);

public:
	ARampageSlash();

	UFUNCTION()
	void Stop();

	UPROPERTY(BlueprintAssignable)
	FOnReSpawn	mReSpawn;

	UPROPERTY(BlueprintAssignable)
		FOnHit	mOnHit;

	UFUNCTION()
	void ResetSlash(AActor* preParticle);

	UFUNCTION()
	void MoveStart(ACharacter* target);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


public:
	void SetIsEnableMove(bool _value) { mIsEnableMove = _value; }


	UPROPERTY()
		FTimerHandle mTimerHandle2;

protected:
	bool	mIsEnableMove;
	ACharacter* mTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
		TSubclassOf<UCameraShakeBase> mHitShake;
};
