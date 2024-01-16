// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "GameFramework/Actor.h"
#include "ItemActor.generated.h"

UCLASS()
class AG_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemActor();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;


public:
	UStaticMeshComponent* GetMesh()
	{ 
		return mMesh; 
	}

	UBoxComponent* GetBoxComponent() 
	{
		return mBox; 
	}

	void SetStaticMesh(const FString& _path);
	void SetItemId(EITEM_ID _id) 
	{
		mItemId = _id; 
	}


	float TransformedSin();

	void SetOverlapEnable()
	{
		mBox->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OnOverlap);
	}

protected:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* mMesh;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	UBoxComponent* mBox;

	UPROPERTY()
	FTimerHandle mTimerHandle;

	EITEM_ID	mItemId;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine params")
		float	amplitude = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine params")
		float	timeConstant = 5.f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float	runningTime = 0.f;
};
