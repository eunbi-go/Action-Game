// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "../ParticleNiagara.h"
#include "GameplayEffectTypes.h"
#include "ValkyrieProjectile.generated.h"

/**
 * 
 */
UCLASS()
class AG_API AValkyrieProjectile : public AParticleNiagara
{
	GENERATED_BODY()
public:
	AValkyrieProjectile();
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* mProjectileMovement;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn=true))
	FGameplayEffectSpecHandle mDamageEffectSpecHandle;



protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* mSphere;

	UPROPERTY(VisibleAnywhere)
	USoundBase* mImpactSound;

	UPROPERTY(VisibleAnywhere)
	USoundBase* mLoopingSound;

	UPROPERTY()
	UAudioComponent* mLoopingSoundComp;

	UPROPERTY(VisibleAnywhere)
	UNiagaraSystem* mImpactEffect;

	bool	mIsOverlap = false;
	UPROPERTY(EditDefaultsOnly)
	float	mLifeSpan = 10.f;
};
