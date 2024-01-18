// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Monster.h"
#include "FengMao.generated.h"

/**
 * 
 */
UCLASS()
class AG_API AFengMao : public AMonster
{
	GENERATED_BODY()
	
		DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkill1Start, class AActor*, actor);
		DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkill1MoveStart);
		DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDecalDeath);

public:
	AFengMao();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController);
	virtual void UnPossessed();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void NormalAttackCheck();

public:
	UPROPERTY(BlueprintAssignable)
	FOnSkill1Start	mSkill1Start;

	UPROPERTY(BlueprintAssignable)
		FOnSkill1MoveStart	mSkill1MoveStart;
	
	UPROPERTY(BlueprintAssignable)
	FDecalDeath mDecalDeath;

	virtual void Skill1()override;
	virtual void Skill2()override;
	UFUNCTION()
	virtual void Skill3()override; 

	UFUNCTION()
	void SpawnSkill3();

	virtual void Skill4()override;

	void SkillCollisionCheck(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void PlaySkillMontage(MONSTER_MOTION motion) override;


	UFUNCTION()
	void Temp(class ACollisionObject* collisionObject, const FHitResult& Hit, AActor* hitActor);
	UFUNCTION()
	void CameraShake(class AMeteo* niagara);
	UFUNCTION()
	void RespawnSkill4(ARockBurst* particle);

	UFUNCTION()
	void RespawnSkill1();

	UFUNCTION()
		void Hit(FVector impactPoint);

public:
	void EndSkill4() 
	{ 
		isEnableSkill4Respawn = false; 
	}

	void StartJump();
	void StopJump();


	


public:
	UPROPERTY()
	FTimerHandle mTimerHandle;

	UPROPERTY()
	FTimerHandle mSkillTimerHandle;

	int32	mSkill3SpawnCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	TSubclassOf<UCameraShakeBase> mMeteoCameraShake;

	int32	mSkill4Count;

	FVector originalPos;
	bool	isEnableSkill4Respawn;

	int32	mSkill1Count;
	bool	isEnableSkill1Respawn;
	FVector	mSkill1CenterPosition;
	FVector mSkill1TargetPosition;

	TArray<FVector> skill3PositionArray;
	TArray<class ADecal*> skill3DecalArray;
	int32	mSkill3Index;

	class ARampageSlash* mPreSlash;

	float	mMeteoRangeWidth = 250.f;
	float	mMetoRangeHeight = 100.f;
};
