// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "../ParticleNiagara.h"
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

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* mProjectileMovement;


protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* mSphere;
};
