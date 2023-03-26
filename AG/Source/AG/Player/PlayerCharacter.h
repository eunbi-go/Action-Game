// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class AG_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	void SetRunStateSpeed() { GetCharacterMovement()->MaxWalkSpeed = 1000.f; }
	void SetWalkStsteSpeed() { GetCharacterMovement()->MaxWalkSpeed = 600.f; }

public:
	void MoveForward(float _scale);
	void MoveHorizontal(float _scale);
	void MouseRotateY(float _scale);
	void MouseRotateZ(float _scale);

	void ChangePlayModeKey();
	void EvadeKey();
	void EvadeBackwardKey();
	void JumpKey();
	void JumpEnd();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	USpringArmComponent* mSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	UCameraComponent* mCamera;

	class UPlayerAnimInstance* mAnimInst;
};
