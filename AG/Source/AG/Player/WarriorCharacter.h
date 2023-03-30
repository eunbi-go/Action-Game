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

protected:
	virtual void BeginPlay() override;


public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



public:
	virtual void NormalAttackCheck();
	virtual void Skill1();
	virtual void Skill2();

	virtual void UseSkill(SKILL_TYPE _skillType) override;
	virtual void SpawnSkill(SKILL_TYPE _skillType, int32 _skillInfoArrayIndex) override;
	
	virtual void Gauge(float _scale) override;
	virtual void GaugeEnd() override;
	virtual void StopLaunchCharacter() override;
	virtual void RestartSkill() override;

public:
	


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	TSubclassOf<UCameraShakeBase> mNormalAttackShake;

	bool mIsGaugeEnd = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
		class ATemporaryfCameraActor* mTempCamera;
};
