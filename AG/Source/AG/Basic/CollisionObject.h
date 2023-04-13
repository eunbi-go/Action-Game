// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "GameFramework/Actor.h"
#include "CollisionObject.generated.h"

UCLASS()
class AG_API ACollisionObject : public AActor
{
	GENERATED_BODY()
	
		DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHited, ACollisionObject*, niagara, const FHitResult&, Hit, AActor*, hitActor);

public:	
	ACollisionObject();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(BlueprintAssignable)
		FOnHited	mOnHitt;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UBoxComponent* mCollisionBox;

	UPROPERTY()
	FTimerHandle mTimerHandle;

	bool	mIsEnd;
	float	mDeadTime;
};
