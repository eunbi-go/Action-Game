// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "UObject/Interface.h"
#include "HitInterface.generated.h"

UINTERFACE(MinimalAPI)
class UHitInterface : public UInterface
{
	GENERATED_BODY()
};

class AG_API IHitInterface
{
	GENERATED_BODY()

public:
	virtual void GetHit(const FVector& _impactPoint) = 0;
};
