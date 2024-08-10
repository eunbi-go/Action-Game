// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "../ParticleNiagara.h"
#include "GameplayEffectTypes.h"
#include "ValkyrieSprintEffect.generated.h"

/**
 * 
 */
UCLASS()
class AG_API AValkyrieSprintEffect : public AParticleNiagara
{
	GENERATED_BODY()
public:
	AValkyrieSprintEffect();
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle mDamageEffectSpecHandle;

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* mSphere;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* mImpactEffect;

	bool	mIsOverlap = false;
};
