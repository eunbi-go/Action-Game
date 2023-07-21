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

protected:
	virtual void BeginPlay() override;

	virtual void PlayMontage(FName _montageName, FName _sectionName = "");
	virtual void EquipWeaponKey() override;
	virtual void NormalAttackKey() override;
	void Skill1Key();

	void NormalAttackStart();
	void NormalAttackEnd();

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

	UPROPERTY(VisibleAnywhere, Category = Component, meta = (AllowPrivateAccess = true))
		AActor* mCameraTwo;

	float timeToNextCameraChange = 1.f;
};
