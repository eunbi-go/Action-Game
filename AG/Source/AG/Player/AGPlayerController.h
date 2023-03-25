// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "GameFramework/PlayerController.h"
#include "AGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AG_API AAGPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAGPlayerController();


protected:
	virtual void BeginPlay() override;

	// 컴포넌트들이 모두 초기화된 다음 호출되는 함수
	virtual void PostInitializeComponents() override;

protected:
	// 플레이어 컨트롤러가 Pawn에 빙의될 때 호출
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

public:
	virtual void Tick(float DeltaTime) override;
};
