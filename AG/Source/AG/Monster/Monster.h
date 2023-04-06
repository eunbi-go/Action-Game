// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "GameFramework/Character.h"
#include "Monster.generated.h"

UCLASS()
class AG_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	AMonster();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController);
	virtual void UnPossessed();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);


public:
	void GoNextPatrolPoint();


public:
	void SetSpawnPoint(class AMonsterSpawnPoint* _spawnPoint) { mSpawnPoint = _spawnPoint; }
	void SetIsAttackEnd(bool _value) { mIsAttackEnd = _value; }
	void SetPatrolPointPosition(const TArray<FVector>& _array) { mPatrolPointPositionArray = _array; }
	void SetPatrolDirection(PATROL_END_DIRECTION _direction) { mPatrolDirection = _direction; }
	void ClearPatrolWaitTime() { mPatrolWaitTime = 0.f; }
	void SetPatrolWaitTime(float _value) { mPatrolWaitTime += _value; }

public:
	const FMonsterInfo& GetMonsterInfo() const { return mInfo; }
	class UMonsterAnimInstance* GetMonsterAnimInst() { return mAnimInst; }
	bool GetIsAttackEnd() { return mIsAttackEnd; }
	float GetPatrolWaitTime() { return mPatrolWaitTime; }
	bool GetIsPatrolEnable() { return mPatrolPointPositionArray.Num() >= 2; }
	const FVector& GetPatrolPosition() { return mPatrolPointPositionArray[mPatrolIndex]; }
	int GetPatrolIndex() { return mPatrolIndex; }


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FMonsterInfo	mInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FName			mMonsterTableRowName;

	// �ڽ��� ������ ���� ����Ʈ.
	class AMonsterSpawnPoint* mSpawnPoint;

	class UMonsterAnimInstance* mAnimInst;


protected:
	// ������ ���� ������ �����°�.
	bool	mIsAttackEnd;


protected:
	TArray<FVector>	mPatrolPointPositionArray;

	PATROL_END_DIRECTION	mPatrolDirection;

	int32	mPatrolIndex;

	float	mPatrolWaitTime;

	float	mPatrolProgressTime;
};
