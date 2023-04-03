// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerCharacter.h"
#include "WarriorCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AG_API AWarriorCharacter : public APlayerCharacter
{
	GENERATED_BODY()
	
public:
	AWarriorCharacter();


public:
	UFUNCTION()
		void Skill1End(class ASkillActor* SkillActor, const FHitResult& Hit);
	UFUNCTION()
		void Skill3End(class ASkillActor* SkillActor, const FHitResult& Hit);


protected:
	virtual void BeginPlay() override;


public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



public:
	virtual void NormalAttackCheck();
	virtual void Skill1();
	virtual void Skill2();
	virtual void Skill3();
	virtual void Skill4();

	virtual void UseSkill(SKILL_TYPE _skillType) override;
	virtual void SpawnSkill(SKILL_TYPE _skillType, int32 _skillInfoArrayIndex) override;
	void ApplySkill(SKILL_TYPE _skillType);
	virtual void EndSkill(SKILL_TYPE _skillType) override;

	virtual void Gauge(float _scale) override;
	virtual void GaugeEnd() override;
	virtual void StopLaunchCharacter() override;
	virtual void RestartSkill() override;
	

	void NextSprint();
	void TempCameraOnOff(bool _value);
	void SprintJumpStart();
	void FinishSprint();
	void StartSlashCameraShake();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	TSubclassOf<UCameraShakeBase> mNormalAttackShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
		TSubclassOf<UCameraShakeBase> mContinuousShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
		TSubclassOf<UCameraShakeBase> mSlashShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	class ATemporaryfCameraActor* mTempCamera;


private:
	// true: 게이지 차징이 끝난 상태, 목표 지점으로 빠르게 이동해야 함.
	// false: 디폴트 값.
	bool	mIsGaugeEnd;
	FVector	mSprintDirection;

	float	mContinuousTime;
};
