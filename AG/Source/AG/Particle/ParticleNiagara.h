// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ParticleActor.h"
#include "ParticleNiagara.generated.h"

/**
 * 
 */
UCLASS()
class AG_API AParticleNiagara : public AParticleActor
{
	GENERATED_BODY()
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHitedd, class ACollisionObject*, collisionObject, const FHitResult&, Hit, AActor*, hitActor);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraShake, class AParticleNiagara*, niagara);

public:
	AParticleNiagara();

protected:
	virtual void BeginPlay() override;


public:
	virtual void SetParticle(UNiagaraSystem* _particle);
	virtual void SetParticle(const FString& _path);

public:
	UFUNCTION()
	void ParticleFinish(UNiagaraComponent* _particle);

	UFUNCTION()
	void Check();

	UFUNCTION()
	void Temp(class ACollisionObject* collisionObject, const FHitResult& Hit, AActor* hitActor);

public:
	UNiagaraComponent* GetNiagara() { return mParticle; }

	UPROPERTY(BlueprintAssignable)
		FOnHitedd	mOnHittd;

	UPROPERTY(BlueprintAssignable)
	FOnCameraShake	mCameraShake;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UNiagaraComponent* mParticle;

	UPROPERTY()
	FTimerHandle mTimerHandle;

public:
	UPROPERTY()
	class ACollisionObject* collObj;
};
