// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "GameFramework/Actor.h"
#include "Decal.generated.h"

UCLASS()
class AG_API ADecal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADecal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	void SetDecalRotation(const FRotator& Rot)
	{
		mDecal->SetRelativeRotation(Rot);
	}

	void SetDecalMaterial(const FString& Path);
	void SetDecalVisibility(bool Visible)
	{
		mDecal->SetVisibility(Visible);
	}



protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		USceneComponent* mRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		UDecalComponent* mDecal;
};
