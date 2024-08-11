// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ParticleNiagara.h"
#include "Meteo.generated.h"

/**
 * 
 */
UCLASS()
class AG_API AMeteo : public AParticleNiagara
{
	GENERATED_BODY()
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHitedd, class ACollisionObject*, collisionObject, const FHitResult&, Hit, AActor*, hitActor);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraShake, class AMeteo*, niagara);

public:
	AMeteo();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
		void ParticleFinish(UNiagaraComponent* _particle);

	UFUNCTION()
	void SpawnCollisionActor();

	UFUNCTION()
		void Temp(class ACollisionObject* collisionObject, const FHitResult& Hit, AActor* hitActor);

public:
	UPROPERTY(BlueprintAssignable)
		FOnHitedd	mOnHittd;

	UPROPERTY(BlueprintAssignable)
		FOnCameraShake	mCameraShake;

	UPROPERTY()
		FTimerHandle mTimerHandle;

public:
	UPROPERTY()
		class ACollisionObject* collObj;

private:
	bool mIsSpawnCollisionActor = false;
};
