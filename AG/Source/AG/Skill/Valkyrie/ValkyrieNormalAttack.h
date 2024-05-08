// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "../AGSkillActor.h"
#include "ValkyrieNormalAttack.generated.h"

/**
 * 
 */
UCLASS()
class AG_API AValkyrieNormalAttack : public AAGSkillActor
{
	GENERATED_BODY()
public:
	AValkyrieNormalAttack();
	virtual void Activate() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SpawnEffect() override;
	virtual void SkillEnd() override;

	void InputPressed();
	void Notify_NextAttackCheck();
	void Notify_AttackEnd();
	void EndNormalAttack();
	void StartNormalAttack();
protected:
	virtual void BeginPlay() override;


	//-------------------------------
	// Normal Attack
	//-------------------------------
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack")
	bool mIsAttacking = false;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack")
	bool mIsCanNextAttack = false;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack")
	bool mIsAttackInputOn = false;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack")
	int32 mCurrentAttackIndex = 1;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack")
	int32 mAttackMaxIndex = 4;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Collision")
	class ACollisionActor* mCollisionActor;


	
};
