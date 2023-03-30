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

	UCameraComponent* GetCamera() { return mCamera; }

protected:
	UCameraComponent* mCamera;
};
