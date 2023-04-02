// Fill out your copyright notice in the Description page of Project Settings.


#include "TemporaryfCameraActor.h"

ATemporaryfCameraActor::ATemporaryfCameraActor()
{
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TempororyCamera"));
	mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

	//SetRootComponent(mSpringArm);
	//mCamera->SetupAttachment(mSpringArm);
}
