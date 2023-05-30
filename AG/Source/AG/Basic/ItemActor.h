// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "GameFramework/Actor.h"
#include "ItemActor.generated.h"

UCLASS()
class AG_API AItemActor : public AActor
{
	GENERATED_BODY()
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHited, AItemActor*, niagara, const FHitResult&, Hit, AActor*, hitActor);

public:	
	AItemActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UStaticMeshComponent* GetMesh()
	{ 
		return mMesh; 
	}

	UBoxComponent* GetBoxComponent() 
	{
		return mBox; 
	}

public:
	void SetStaticMesh(const FString& _path);
	void SetItemId(EITEM_ID _id) 
	{
		mItemId = _id; 
	}

	UPROPERTY(BlueprintAssignable)
		FOnHited	mOnHitt;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* mMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UBoxComponent* mBox;

	UPROPERTY()
	FTimerHandle mTimerHandle;

	EITEM_ID	mItemId;
};
