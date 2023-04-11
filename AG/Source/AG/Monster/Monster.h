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
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController);
	virtual void UnPossessed();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);


public:
	virtual void NormalAttackCheck();

public:
	void GoNextPatrolPoint();


public:
	void SetSpawnPoint(class AMonsterSpawnPoint* _spawnPoint) { mSpawnPoint = _spawnPoint; }
	void SetIsAttackEnd(bool _value) { mIsAttackEnd = _value; }
	
	void SetPatrolPointPosition(const TArray<FVector>& _array) { mPatrolPointPositionArray = _array; }
	void SetPatrolDirection(PATROL_END_DIRECTION _direction) { mPatrolDirection = _direction; }
	void ClearPatrolWaitTime() { mPatrolWaitTime = 0.f; }
	void SetPatrolWaitTime(float _value) { mPatrolWaitTime += _value; }
	void SetPatrolType(PATROL_TYPE _type) { mPatrolType = _type; }

	void SetIsPatrolEnable(bool _value) { mIsPatrolEnable = _value; }

	void SetPatrolSplineLength(float _value) { mPatrolSplineLength = _value; }
	void SetPatrolCellDistance(float _value) { mPatrolCellDistance = _value; }
	void SetPatrolSplineCount(int _value) { mPatrolSplineCount = _value; }

public:
	const FMonsterInfo&			GetMonsterInfo() const { return mInfo; }
	class UMonsterAnimInstance* GetMonsterAnimInst() { return mAnimInst; }
	bool						GetIsAttackEnd() { return mIsAttackEnd; }

	float	GetPatrolWaitTime() { return mPatrolWaitTime; }
	bool	GetPatrolEnable() { return mPatrolPointPositionArray.Num() >= 2; }

	FVector GetPatrolPosition() const;
	FVector GetPatrolPointPosition() const;
	bool	GetIsPatrolPointArrive();

	int		GetPatrolIndex() { return mPatrolIndex; }
	PATROL_TYPE GetPatrolType() { return mPatrolType; }
	bool	GetIsPatrolEnable() { return mIsPatrolEnable; }

	int32	GetUsingSkillIndex() { return mUsingSkillIndex; }
	const FMonsterSkillInfo* GetSkillInfo();
		

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FMonsterInfo	mInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FName			mMonsterTableRowName;

	// 자신이 스폰된 스폰 포인트.
	class AMonsterSpawnPoint* mSpawnPoint;

	class UMonsterAnimInstance* mAnimInst;

	AActor* mHitActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UWidgetComponent* mWidgetComopnent;

	


protected:
	// 몬스터의 현재 공격이 끝났는가.
	bool	mIsAttackEnd;


protected:
	TArray<FVector>	mPatrolPointPositionArray;

	PATROL_END_DIRECTION	mPatrolDirection;
	PATROL_TYPE	mPatrolType;

	int32	mPatrolIndexAddValue;

	int32	mPatrolIndex;
	float	mPatrolWaitTime;
	float	mPatrolProgressTime;
	bool	mIsPatrolEnable;


	// 스플라인을 총 몇 개의 CP 로 쪼갰는가.
	int32	mPatrolSplineCount;
	float	mPatrolSplineLength;
	float	mPatrolCellDistance;
	// 현재까지 이동한 거리.
	float	mPatrolCurrDistance;



protected:
	//------------------
	// 스킬.
	//------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FMonsterSkillInfo>	mSkillInfoArray;

	int32	mUsingSkillIndex;

	TArray<FName>	mSkillNameArray;
};
