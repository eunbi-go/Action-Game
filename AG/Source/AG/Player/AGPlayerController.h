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

	// ������Ʈ���� ��� �ʱ�ȭ�� ���� ȣ��Ǵ� �Լ�
	virtual void PostInitializeComponents() override;

protected:
	// �÷��̾� ��Ʈ�ѷ��� Pawn�� ���ǵ� �� ȣ��
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

public:
	virtual void Tick(float DeltaTime) override;
};
