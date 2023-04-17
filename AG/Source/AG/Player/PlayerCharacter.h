// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class AG_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCoin, class AActor*, actor);

public:
	APlayerCharacter();

	UPROPERTY(BlueprintAssignable)
	FOnCoin	mGetCoin;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	
public:
	//---------------------
	// 공격, 스킬 관련 가상 함수들.
	//---------------------
	virtual void NormalAttackCheck();
	virtual void Skill1();
	virtual void Skill2();
	virtual void Skill3();
	virtual void Skill4();
	virtual void EndSkill(SKILL_TYPE _skillType);

	virtual void UseSkill(SKILL_TYPE _skillType);
	virtual void SpawnSkill(SKILL_TYPE _skillType, int32 _skillInfoArrayIndex);

	virtual void GaugeEnd();
	virtual void StopLaunchCharacter();
	void virtual RestartSkill();



public:
	void EquipWeaponToHand();
	void SetWeaponTrailOnOff(bool _value);
	void ClickDestination();

public:
	void SetRunStateSpeed() { GetCharacterMovement()->MaxWalkSpeed = 1000.f; }
	void SetWalkStsteSpeed() { GetCharacterMovement()->MaxWalkSpeed = 600.f; }
	void SetDashStsteSpeed() { GetCharacterMovement()->MaxWalkSpeed = 1500.f; }
	void SetCustomTimeDilation(float _value) { CustomTimeDilation = _value; }
	void SetIsDelay(bool _isDelay) { mIsDelay = _isDelay; }
	void SetTargetPosition(FVector _position) { mTargetPosition = _position; }
	void SetISEnableSprint(bool _value) { isSprint = _value; }
	void SetCoin(int32 _value);

public:
	float GetTeleportGaueTime() { return mTeleportGauge; }
	int GetSprintCount() { return mSprintCount; }
	bool GetIsSprint() { return isSprint; }
	class UPlayerAnimInstance* GetAnimInstance() {
		return mAnimInst;
	}
public:
	//---------------------
	// 키 입력 함수들.
	//---------------------
	void MoveForward(float _scale);
	void MoveHorizontal(float _scale);
	void MouseRotateY(float _scale);
	void MouseRotateZ(float _scale);
	void Dash(float _scale);
	virtual void Gauge(float _scale);

	void ChangePlayModeKey();
	void EvadeKey();
	void EvadeBackwardKey();
	void JumpKey();
	void JumpEnd();
	void EquipWeaponKey();
	void NormalAttackKey();
	void Skill1Key();
	void Skill2Key();
	void Skill3Key();
	void Skill4Key();
	void InventoryOnOffKey();



protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	USpringArmComponent*		mSpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	UCameraComponent*			mCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	class UCharacterStatComponent* mStat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = true))
	TArray<FSkillInfo>	mSkillInfoArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	UNiagaraComponent* mDash;

	class UPlayerAnimInstance*	mAnimInst;

	UPROPERTY()
	FTimerHandle timerHandle;

	AActor* mHitActor;


protected:
	class AWeaponActor* mWeapon;

	// 현재 무기를 장착하고 있는지.
	bool	mIsEquipWeapon;




protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	FPlayerInfo		mInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FName			mPlayerTableRowName;

	bool	mIsDelay;

	float	mTeleportGauge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
		TSubclassOf<UCameraShakeBase> mGaugeShake;

	FVector	mTargetPosition;
	float dist = 5000.0f;

	int mSprintCount;
	bool isSprint;
};
