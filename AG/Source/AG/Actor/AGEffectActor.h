// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "GameFramework/Actor.h"
#include "AGEffectActor.generated.h"

UCLASS()
class AG_API AAGEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAGEffectActor();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* mSphere;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* mMesh;
};
