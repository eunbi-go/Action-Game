// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "../ParticleNiagara.h"
#include "GameplayEffectTypes.h"
#include "RangeEffect.generated.h"

/**
 * 
 */
UCLASS()
class AG_API ARangeEffect : public AParticleNiagara
{
	GENERATED_BODY()
public:
	ARangeEffect();
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle mDamageEffectSpecHandle;

	void Active();

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* mBox;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* mImpactEffect;

	bool	mIsOverlap = false;
};
