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
	void Skill1End(class ASkillActor* SkillActor, UParticleSystemComponent* comp);
	
	UFUNCTION()
	void Skill2EndWithNiagara(class ASkillActor* SkillActor, UNiagaraComponent* comp);

	UFUNCTION()
	void Skill3EndWithNiagara(class ASkillActor* SkillActor, UNiagaraComponent* comp);

	UFUNCTION()
	void Skill4EndWithNiagara(class ASkillActor* SkillActor, UNiagaraComponent* comp);

protected:
	virtual void BeginPlay() override;


public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



public:
	virtual void NormalAttackCheck();
	virtual void SprintAttackCheck();

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
	void SpawnFresnel();
	void ResetFresnel();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	TSubclassOf<UCameraShakeBase> mNormalAttackShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
		TSubclassOf<UCameraShakeBase> mContinuousShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
		TSubclassOf<UCameraShakeBase> mSlashShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	class ATemporaryfCameraActor* mTempCamera;

	USkeletalMesh* mGhostMesh;


private:
	// true: ������ ��¡�� ���� ����, ��ǥ �������� ������ �̵��ؾ� ��.
	// false: ����Ʈ ��.
	bool	mIsGaugeEnd;
	FVector	mSprintDirection;

	float	mContinuousTime;

	bool	mFresnelEnable;
	float	mFresnelTime;
	float	mFresnelTimeEnd;
	
	float	mFresnelCreateTime;
	float	mFresnelCreateTimeEnd;
};
