// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "GameFramework/Actor.h"
#include "BasicActor.generated.h"

UCLASS()
class AG_API ABasicActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABasicActor();


protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


public:
	USkeletalMeshComponent* GetMesh() 
	{ 
		return mMesh;
	}


public:
	void SetSkeletalMesh(const FString& _path);



protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* mMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UBoxComponent* mBox;
};
