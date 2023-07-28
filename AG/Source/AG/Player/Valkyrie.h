// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "AGPlayer.h"
#include "Valkyrie.generated.h"

class UMotionWarpingComponent;

UCLASS()
class AG_API AValkyrie : public AAGPlayer
{
	GENERATED_BODY()

public:
	AValkyrie();

	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void UnequipSword();
	
	virtual void SpawnEffect() override;


protected:
	virtual void BeginPlay() override;

	virtual void PlayMontage(FName _montageName, FName _sectionName = "");
	virtual void EquipWeaponKey() override;
	virtual void NormalAttackKey() override;
	void Skill1Key();

	void NormalAttackStart();
	void NormalAttackEnd();

	UFUNCTION()
	void SetMontagePlayRate();


	class ASword* mWeapon;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack")
	bool mIsCanNextAttack;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack")
		bool mIsAttackInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack")
		int32 mCurrentAttackIndex;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack")
		int32 mAttackMaxIndex;

	UPROPERTY();
	TObjectPtr<UMotionWarpingComponent> mMotionWarpComp;

	UPROPERTY(VisibleAnywhere, Category = Component, meta = (AllowPrivateAccess = true))
	UCameraComponent* mCameraOne;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	mJumpAttackIndex;


private:
	void SetAnimDelegate();

	FVector tempLocation;

	bool	mIsJumpAttack = false;

public:
	bool GetIsJumpAttackEnable() { return mActionState == EActionState::EAS_JumpAttack; }
	EActionState GetActionState() { return mActionState; }
	int32 GetJumpAttackIndex() { return mJumpAttackIndex; }
};
