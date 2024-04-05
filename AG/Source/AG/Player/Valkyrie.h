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
class AG_API AValkyrie : public AAGPlayer, public IHitInterface
{
	GENERATED_BODY()

public:
	AValkyrie();

	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual void SpawnEffect() override;

	void Delay(float _customTimeDilation, float _timeRate, bool _isLoop);
	void UnequipSword();

	virtual void GetHit(const FVector& _impactPoint) override;
	
	//-------------------------------
	// Combat Interface
	//-------------------------------
	virtual int32 GetPlayerLevel() override;


private:
	virtual void InitAbilityActorInfo() override;
	void SetAnimDelegate();
	void PrintAllActionState();


protected:
	virtual void BeginPlay() override;
	
	//-------------------------------
	// Input Functions
	//-------------------------------
	virtual void EquipWeaponKey() override;
	virtual void NormalAttackKey() override;
	virtual void JumpKey() override;
	void Skill1Key();
	void Skill2Key();
	void Skill2KeyUp();
	void Skill2KeyPressing();
	void Skill3Key();
	void Skill4Key();
	void TargetingKey();
	virtual void CrouchKey() override;
	virtual void GuardKey() override;


	//-------------------------------
	// Animation Montage Functions
	//-------------------------------
	virtual void PlayMontage(FName _montageName, FName _sectionName = "");
	UFUNCTION()
	void SetMontagePlayRate();


	//-------------------------------
	// Normal Attack Functions
	//-------------------------------
	void NormalAttackStart();
	void NormalAttackEnd();


	//-------------------------------
	// Fresnel Functions
	//-------------------------------
	void SpawnFresnel();
	void ResetFresnel();


	//-------------------------------
	// Timeline/Curve Functions
	//-------------------------------
	UFUNCTION()
	void TimeLineFinish();
	UFUNCTION()
	void CurveUpdate(float value);
	UFUNCTION()
	void CrouchCurveUpdate(float value);

	void CameraSwitch(bool _value);


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CameraSwitch", meta = (AllowPrivateAccess = true))
	class ASword* mWeapon;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	class AShield* mGuardShield;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Fresnel", meta = (AllowPrivateAccess = true))
	USkeletalMesh* mGhostMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	TSubclassOf<UCameraShakeBase> mCameraShake;

	//-------------------------------
	// Components
	//-------------------------------
	UPROPERTY();
	TObjectPtr<UMotionWarpingComponent> mMotionWarpComp;

	UPROPERTY(VisibleAnywhere, Category = Component, meta = (AllowPrivateAccess = true))
	UTargetingComponent* mTargetingComp;

	


	//-------------------------------
	// Normal Attack
	//-------------------------------
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack")
	bool mIsCanNextAttack;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack")
	bool mIsAttackInputOn;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack")
	int32 mCurrentAttackIndex;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack")
	int32 mAttackMaxIndex;



	//-------------------------------
	// Timeline/Curve
	//-------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CameraSwitch", meta = (AllowPrivateAccess = true))
	UTimelineComponent* mTimeLineComp;
	// 기존 카메라에서 Switch 될 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CameraSwitch", meta = (AllowPrivateAccess = true))
	UCameraComponent* mTempCameraComp;
	// 기존 카메라의 위치 저장
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CameraSwitch", meta = (AllowPrivateAccess = true))
	UCameraComponent* mCameraCompRef;
	// mTempCameraComp 의 Transform 저장
	FTransform mTempCameraTrans;
	// mCameraComp/mCameraCompRef 의 Transform 저장
	FTransform mCameraCompTrans;
	// Timeline 이 끝났을 때/업데이트될 때 호출되는 함수를 바인딩하기 위한 델리게이트
	FOnTimelineEvent mTimelineFinishDelegate;
	FOnTimelineFloat mCurveUpdateDelegate;
	// Timeline 에 사용할 Curve
	UCurveFloat* mTimeLineCurve;
	// CameraComp -> TempCameraComp : false
	// TempCameraComp -> CameraComp : true
	bool mToCameraComp = true;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CameraSwitch", meta = (AllowPrivateAccess = true))
	UTimelineComponent* mCrouchTimeLineComp;
	FOnTimelineFloat mCrouchCurveUpdateDelegate;
	

	

private:
	FresnelInfo mFresnelInfo;

	float	mCrouchCapsuleHalfHeight;
	float	mCapsuleHalfHeight;
	
	int32	mSlashSkillIndex = -1;
	bool	mIsNextSlashEnable = false;
	bool	mIsNextSlashInput = false;
	bool	mIsSlash = false;

	float	mSkill2PressingTime = 0.f;
};
