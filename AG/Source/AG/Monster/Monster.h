// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "GameFramework/Character.h"
#include "../Interface/HitInterface.h"
#include "../CharacterBase/AGBaseCharacter.h"
#include "Monster.generated.h"

UENUM()
enum class EHitActionType : uint8
{
	EHAT_Idle,
	EHAT_Jump
};

UENUM()
enum class EAttackType : uint8
{
	EAT_Ing,
	EAT_End
};

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class AG_API AMonster : public AAGBaseCharacter, public IHitInterface
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

	virtual void GetHit(const FVector& _impactPoint) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	FOnAttackEndDelegate	mOnAttackEnd;

private:
	void PlayHitMontage(const FVector& _impactPoint);


public:
	virtual void Skill1();
	virtual void Skill2();
	virtual void Skill3();
	virtual void Skill4();

//private:
public:
	void UseSkill(float _deltaTime);
	void CheckSkillCoolTime(float _deltaTime);
	void SelectSkill(TArray<int32> _enableSkillIndexArray);
	void SetRotationToTarget();

public:
	virtual void NormalAttackCheck();
	void ClearUsingSkill();
	void ClearAllSkill();

public:
	void GoNextPatrolPoint();
	void DestroyMonster();

public:
	void SetSpawnPoint(class AMonsterSpawnPoint* _spawnPoint) 
	{
		mSpawnPoint = _spawnPoint; 
	}

	void SetIsAttackEnd(bool _value) 
	{ 
		//PrintViewport(3.f, FColor::Red, TEXT("setAttack")); 
		mIsAttackEnd = _value; 
	}
	
	void SetPatrolPointPosition(const TArray<FVector>& _array) 
	{
		mPatrolPointPositionArray = _array; 
	}

	void SetPatrolDirection(PATROL_END_DIRECTION _direction)
	{
		mPatrolDirection = _direction; 
	}

	void ClearPatrolWaitTime()
	{
		mPatrolWaitTime = 0.f; 
	}

	void SetPatrolWaitTime(float _value) 
	{
		mPatrolWaitTime += _value; 
	}

	void SetPatrolType(PATROL_TYPE _type) 
	{
		mPatrolType = _type; 
	}

	void SetIsPatrolEnable(bool _value)
	{
		mIsPatrolEnable = _value; 
	}

	void SetPatrolSplineLength(float _value)
	{
		mPatrolSplineLength = _value; 
	}

	void SetPatrolCellDistance(float _value)
	{
		mPatrolCellDistance = _value; 
	}

	void SetPatrolSplineCount(int _value)
	{
		mPatrolSplineCount = _value; 
	}

public:
	const FMonsterInfo&	GetMonsterInfo()const 
	{
		return mInfo; 
	}

	class UMonsterAnimInstance* GetMonsterAnimInst()
	{
		return mAnimInst; 
	}

	bool GetIsAttackEnd()
	{
		return mIsAttackEnd; 
	}

	float GetPatrolWaitTime()
	{ 
		return mPatrolWaitTime; 
	}

	bool GetPatrolEnable() 
	{
		return mPatrolPointPositionArray.Num() >= 2; 
	}

	FVector GetPatrolPosition() const;
	FVector GetPatrolPointPosition() const;
	bool	GetIsPatrolPointArrive();

	int	GetPatrolIndex() 
	{ 
		return mPatrolIndex; 
	}

	PATROL_TYPE GetPatrolType()
	{
		return mPatrolType; 
	}

	bool GetIsPatrolEnable()
	{
		return mIsPatrolEnable; 
	}

	int32	GetUsingSkillIndex() 
	{ 
		return mUsingSkillIndex; 
	}

	const FMonsterSkillInfo* GetSkillInfo();

	ACharacter* GetBlackboardTarget();
	
	void SetIsDead(bool _value)
	{
		mIsDead = _value;
	}

	bool GetIsDead()
	{
		return mIsDead;
	}

protected:
	bool	mIsDead = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FMonsterInfo	mInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FName			mMonsterTableRowName;

	// �ڽ��� ������ ���� ����Ʈ.
	class AMonsterSpawnPoint* mSpawnPoint;

	class UMonsterAnimInstance* mAnimInst;

	AActor* mHitActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UWidgetComponent* mWidgetComopnent;

	EHitActionType	mHitActionType;

	UPROPERTY()
	FTimerHandle mTimer;

protected:
	// ������ ���� ������ �����°�.
	bool	mIsAttackEnd;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FVector>	mPatrolPointPositionArray;

	PATROL_END_DIRECTION	mPatrolDirection;
	PATROL_TYPE	mPatrolType;

	int32	mPatrolIndexAddValue;

	int32	mPatrolIndex;
	float	mPatrolWaitTime;
	float	mPatrolProgressTime;
	bool	mIsPatrolEnable;


	// ���ö����� �� �� ���� CP �� �ɰ��°�.
	int32	mPatrolSplineCount;
	float	mPatrolSplineLength;
	float	mPatrolCellDistance;
	// ������� �̵��� �Ÿ�.
	float	mPatrolCurrDistance;



protected:
	//------------------
	// ��ų.
	//------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FMonsterSkillInfo>	mSkillInfoArray;

	int32	mUsingSkillIndex;

	TArray<FName>	mSkillNameArray;

	bool	mIsUsingSkill;

	ACharacter* mTarget;

	EAttackType	mAttackType;
};
