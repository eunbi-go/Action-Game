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
	void SetSpawnPoint(class AMonsterSpawnPoint* _spawnPoint) { mSpawnPoint = _spawnPoint; }
	void SetIsAttackEnd(bool _value) { mIsAttackEnd = _value; }

public:
	const FMonsterInfo& GetMonsterInfo() const { return mInfo; }
	class UMonsterAnimInstance* GetMonsterAnimInst() { return mAnimInst; }
	bool GetIsAttackEnd() { return mIsAttackEnd; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FMonsterInfo	mInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FName			mMonsterTableRowName;

	// 자신이 스폰된 스폰 포인트.
	class AMonsterSpawnPoint* mSpawnPoint;

	class UMonsterAnimInstance* mAnimInst;


protected:
	// 몬스터의 현재 공격이 끝났는가.
	bool	mIsAttackEnd;
};
