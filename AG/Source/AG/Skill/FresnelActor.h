// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "Components/PoseableMeshComponent.h"
#include "GameFramework/Actor.h"
#include "FresnelActor.generated.h"

UCLASS()
class AG_API AFresnelActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AFresnelActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


public:
	void SetMesh(const FString& _path);
	void SetMesh(USkeletalMesh* _mesh);
	void CopyPoseFromSkeletalComp(USkeletalMeshComponent* _skeletalMesh);
	void SetFresnelMaterial(const FString& _path);
	void SetFresnelMaterial(UMaterialInterface* _material);



protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UPoseableMeshComponent* mPoseableMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UMaterialInterface* mFresnelMaterial;

	TArray<UMaterialInstanceDynamic*>	mMaterialArray;

protected:
	float	mLifeTime;
	int		mFadeTime;
	int		mFadeTimeMax;
public:
	bool	mIsFade;
};
