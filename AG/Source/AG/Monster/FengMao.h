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
		DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkill1MoveStart, class ACharacter*, target);

public:
	AFengMao();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController);
	virtual void UnPossessed();


public:
	UPROPERTY(BlueprintAssignable)
	FOnSkill1Start	mSkill1Start;

	UPROPERTY(BlueprintAssignable)
		FOnSkill1MoveStart	mSkill1MoveStart;

	virtual void Skill1()override;
	virtual void Skill2()override;
	UFUNCTION()
	virtual void Skill3()override;
	virtual void Skill4()override;

	void SkillCollisionCheck(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);



	UFUNCTION()
	void Temp(class ACollisionObject* collisionObject, const FHitResult& Hit, AActor* hitActor);
	UFUNCTION()
	void CameraShake(class AParticleNiagara* niagara);
	UFUNCTION()
	void RespawnSkill4(ARockBurst* particle);

	UFUNCTION()
	void RespawnSkill1(ARampageSlash* preParticle);

	UFUNCTION()
		void Hit();

public:
	void EndSkill4() { isEnableSkill4Respawn = false; }
	void StartJump();
	void StopJump();


	


public:
	UPROPERTY()
	FTimerHandle mTimerHandle;

	int32	mSkill3SpawnCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	TSubclassOf<UCameraShakeBase> mMeteoCameraShake;

	int32	mSkill4Count;

	FVector originalPos;
	bool	isEnableSkill4Respawn;

	int32	mSkill1Count;
	bool	isEnableSkill1Respawn;
	FVector	mSkill1CenterPosition;
};
