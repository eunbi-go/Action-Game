// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "GameFramework/Actor.h"
#include "Shield.generated.h"

UCLASS()
class AG_API AShield : public AActor
{
	GENERATED_BODY()
	
public:	
	AShield();
	virtual void Tick(float DeltaTime) override;

	void Equip(USceneComponent* Parent, FName SocketName, AActor* NewOwner, APawn* NewInstigator);
	void SetSkeletalMesh(const FString& _path);
	void AttachMeshToSocket(USceneComponent* _parent, const FName& _socketName);
	void SetShieldVisibility(bool Value)
	{
		mMesh->SetVisibility(Value);
	}

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* mMesh;


};
