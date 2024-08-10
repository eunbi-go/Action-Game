// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "../ParticleNiagara.h"
#include "GameplayEffectTypes.h"
#include "ValkyrieFallingSwordEffect.generated.h"

/**
 * 
 */
UCLASS()
class AG_API AValkyrieFallingSwordEffect : public AParticleNiagara
{
	GENERATED_BODY()

public:
	AValkyrieFallingSwordEffect();
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle mDamageEffectSpecHandle;

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
