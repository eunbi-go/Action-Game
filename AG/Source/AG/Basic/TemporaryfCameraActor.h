// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "Camera/CameraActor.h"
#include "TemporaryfCameraActor.generated.h"

/**
 * 
 */
UCLASS()
class AG_API ATemporaryfCameraActor : public ACameraActor
{
	GENERATED_BODY()
	
public:
	ATemporaryfCameraActor();

public:
	void SetRatio(float _ratio) { mCamera->SetAspectRatio(_ratio); }
	void SetCamera(UCameraComponent* _camera) { mCamera = _camera; }
	void SetSpringArm(USpringArmComponent* _springArm) { mSpringArm = _springArm; }

	UCameraComponent* GetCamera() { return mCamera; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
		USpringArmComponent* mSpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	UCameraComponent* mCamera;
};
