// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
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
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
		bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USceneComponent* mRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* mMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UNiagaraComponent* mNiagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UParticleSystemComponent* mParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UBoxComponent* mBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USphereComponent* mSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UCapsuleComponent* mCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EITEM_ID	mItemId;




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
	void SetNiagara(UNiagaraSystem* Niagara);
	void SetNiagara(const FString& Path);
	void SetParticle(UParticleSystem* _particle);
	void SetParticle(const FString& _path);

	void SetItemId(EITEM_ID _id) 
	{
		mItemId = _id; 
	}


	float TransformedSin();

protected:



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine params")
	float	amplitude = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine params")
	float	timeConstant = 5.f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float	runningTime = 0.f;
};
