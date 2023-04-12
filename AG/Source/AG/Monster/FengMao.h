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
	
public:
	AFengMao();

protected:
	virtual void BeginPlay() override;

public:
	virtual void PossessedBy(AController* NewController);
	virtual void UnPossessed();


public:
	virtual void Skill1()override;
	virtual void Skill2()override;
	virtual void Skill3()override;
	virtual void Skill4()override;

	void SkillCollisionCheck(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void Temp(class AParticleNiagara* niagara, const FHitResult& Hit, AActor* hitActor);
};
