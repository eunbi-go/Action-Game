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
	//----------
	// 공격
	//----------
	virtual void NormalAttackCheck();



public:
	void EquipWeaponToHand();



public:
	void SetRunStateSpeed() { GetCharacterMovement()->MaxWalkSpeed = 1000.f; }
	void SetWalkStsteSpeed() { GetCharacterMovement()->MaxWalkSpeed = 600.f; }
	void SetDashStsteSpeed() { GetCharacterMovement()->MaxWalkSpeed = 1500.f; }

public:
	void MoveForward(float _scale);
	void MoveHorizontal(float _scale);
	void MouseRotateY(float _scale);
	void MouseRotateZ(float _scale);
	void Dash(float _scale);

	void ChangePlayModeKey();
	void EvadeKey();
	void EvadeBackwardKey();
	void JumpKey();
	void JumpEnd();
	void EquipWeaponKey();
	void NormalAttackKey();


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	USpringArmComponent*		mSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	UCameraComponent*			mCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
		//UParticleSystemComponent* mDash;
		UNiagaraComponent* mDash;

	class UPlayerAnimInstance*	mAnimInst;




protected:
	class AWeaponActor* mWeapon;

	// 현재 무기를 장착하고 있는지.
	bool	mIsEquipWeapon;
};
