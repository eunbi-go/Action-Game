// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "ParticleNiagara.h"
#include "ValkyrieSlash.generated.h"

/**
 * 
 */
UCLASS()
class AG_API AValkyrieSlash : public AParticleNiagara
{
	GENERATED_BODY()

public:
	AValkyrieSlash();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnParticleOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetDirection(const FVector& _value);
	

	void Stop();
	void Start();

	void SetIsHit(bool _value)
	{
		mIsHit = _value;
	}

	bool GetIsHit()
	{
		return mIsHit;
	}

private:
	FVector mDirection;

	UPROPERTY()
	FTimerHandle	mTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class ACollisionActor* mLeftCollisionActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class ACollisionActor* mCenterCollisionActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class ACollisionActor* mRightCollisionActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		UBoxComponent* mCollisionBox;

	bool mIsHit = false;
};
