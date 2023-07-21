// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "GameFramework/Actor.h"
#include "Sword.generated.h"

UCLASS()
class AG_API ASword : public AActor
{
	GENERATED_BODY()
	
public:	
	ASword();
	void Equip(USceneComponent* _parent, FName _socketName, AActor* _newOwner, APawn* _newInstigator);
	void SetSkeletalMesh(const FString& _path);
	void AttachMeshToSocket(USceneComponent* _parent, const FName& _socketName);

	void ClearIgnoreActors() { mIgnoreActors.Empty(); }

	void SetTrailOnOff(bool _value) { mTrail->SetVisibility(_value); }

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* mMesh;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* mTrail;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* mBoxComp;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* mBoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* mBoxTraceEnd;

	TArray<AActor*> mIgnoreActors;

public:	
	virtual void Tick(float DeltaTime) override;
};
