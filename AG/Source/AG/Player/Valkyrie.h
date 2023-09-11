// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "AGPlayer.h"
#include "Valkyrie.generated.h"

class UMotionWarpingComponent;
class UTargetingComponent;

USTRUCT(BlueprintType)
struct FresnelInfo
{
	GENERATED_USTRUCT_BODY()

public:
	bool	mFresnelEnable;
	float	mFresnelTime;
	float	mFresnelTimeEnd;

	float	mFresnelCreateTime;
	float	mFresnelCreateTimeEnd;
};

UCLASS()
class AG_API AValkyrie : public AAGPlayer
{
	GENERATED_BODY()

public:
	AValkyrie();

	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void SpawnEffect() override;

	void Delay(float _customTimeDilation, float _timeRate, bool _isLoop);
	void UnequipSword();

protected:
	virtual void BeginPlay() override;

	virtual void PlayMontage(FName _montageName, FName _sectionName = "");
	virtual void EquipWeaponKey() override;
	virtual void NormalAttackKey() override;
	void Skill1Key();
	void Skill2Key();
	void Skill3Key();
	void TargetingKey();

	void NormalAttackStart();
	void NormalAttackEnd();

	UFUNCTION()
	void SetMontagePlayRate();

	void SpawnFresnel();
	void ResetFresnel();

	bool mToCameraComp = true;


	UFUNCTION()
	void TimeLineFinish();

	UFUNCTION()
	void CurveUpdate(float value);

	void CameraSwitch(bool _value);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CameraSwitch", meta = (AllowPrivateAccess = true))
	class ASword* mWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CameraSwitch", meta = (AllowPrivateAccess = true))
	USkeletalMesh* mGhostMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	TSubclassOf<UCameraShakeBase> mCameraShake;

	// Components..
	UPROPERTY();
	TObjectPtr<UMotionWarpingComponent> mMotionWarpComp;

	UPROPERTY(VisibleAnywhere, Category = Component, meta = (AllowPrivateAccess = true))
	UTargetingComponent* mTargetingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CameraSwitch", meta = (AllowPrivateAccess = true))
	UTimelineComponent* mTimeLineComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	UCameraComponent* mTempCameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CameraSwitch", meta = (AllowPrivateAccess = true))
	UCameraComponent* mCameraCompRef;


	// Normal Attack
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack")
	bool mIsCanNextAttack;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack")
	bool mIsAttackInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack")
	int32 mCurrentAttackIndex;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack")
	int32 mAttackMaxIndex;


	// Camera Switch
	FTransform mTempCameraTrans;
	FTransform mCameraCompTrans;
	
	FOnTimelineFloat mTimelineUpdateDelegate;
	FOnTimelineEvent mTimelineFinishDelegate;

	
	FOnTimelineFloat timelineFloat;
	UCurveFloat* mTimeLineCurveStart;

	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	mJumpAttackIndex;

	

	

private:
	void SetAnimDelegate();

	FVector tempLocation;

	bool	mIsJumpAttack = false;

	FresnelInfo mFresnelInfo;
	
public:
	bool GetIsJumpAttackEnable() { return mActionState == EActionState::EAS_JumpAttack; }
	EActionState GetActionState() { return mActionState; }
	ESkillState GetSkillState() { return mSkillState; }
	int32 GetJumpAttackIndex() { return mJumpAttackIndex; }
};
