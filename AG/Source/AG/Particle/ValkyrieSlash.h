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
	virtual void SpawnHitEffect(const FVector& Location, const FRotator& Rotator) override;
	
	virtual void OnSystemFinish(class UNiagaraComponent* PSystem) override;

	void SetDirection(const FVector& _value);


	void SetSlashIdx(int32 Idx)
	{
		mSlashIdx = Idx;
	}

private:
	void Initialize();

	FVector mDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	class ACollisionActor* mCollisionActor;

	bool mIsInit = false;

	int32 mSlashIdx = -1;
};
