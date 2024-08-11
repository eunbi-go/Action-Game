// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "GameFramework/Character.h"
#include "../Interface/HitInterface.h"
#include "../CharacterBase/AGBaseCharacter.h"
#include "../Widget/WidgetController/MainWidgetController.h"
#include "../AbilitySystem/Data/CharacterInfo.h"
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
	virtual void InitAbilityActorInfo() override;

public:	
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController);
	virtual void UnPossessed();

	virtual void GetHit(const FVector& _impactPoint) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	virtual void Death() override;

	FOnAttackEndDelegate	mOnAttackEnd;

private:
	void PlayHitMontage(const FVector& _impactPoint);

public:
	virtual void Skill1();
	virtual void Skill2();
	virtual void Skill3();
	virtual void Skill4();

	//-------------------------------------------------------------------
	// Combat Interface
	//-----------------------------
	virtual int32 GetPlayerLevel() override;


	//-------------------------------------------------------------------
	// Boss Skill Behavior Tree
	//-----------------------------
public:
	// 사용할 수 있는 스킬을 체크한다.
	bool CheckEnableSkill();
	// 스킬 사용할 수 있는지 확인만 한다.
	bool IsEnableUseSkill();
	// 스킬 사용.
	const FMonsterSkillInfo* UseSkill();
	// 스킬 쿨타임 체크.
	void CheckSkillCoolTime(float _deltaTime);
	// 스킬 초기화.
	void ClearUsingSkill();
	void ClearAllSkill();
	// 스킬 몽타주 재생.
	virtual void PlaySkillMontage(MONSTER_MOTION motion);

private:
	TArray<int32> enableSkillIndexArray;
	//-------------------------------------------------------------------

	
public:
	virtual void NormalAttackCheck();


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
	
	void SetIsDead(bool _value)
	{
		mIsDead = _value;
	}

	bool GetIsDead()
	{
		return mIsDead;
	}

	

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChange mOnHpChange;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChange mOnMaxHpChange;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Clas")
	int32 mLevel = 1;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montages", meta = (AllowPrivateAccess = true))
	TMap<FName, UAnimMontage*>	mMontages;


	bool	mIsDead = false;
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

	EHitActionType	mHitActionType;

	UPROPERTY()
	FTimerHandle mTimer;

protected:
	// 몬스터의 현재 공격이 끝났는가.
	bool	mIsAttackEnd = true;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<FVector>	mPatrolPointPositionArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterClass mCharacterClass;

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

	bool	mIsUsingSkill;

	ACharacter* mTarget;

	EAttackType	mAttackType;
};
