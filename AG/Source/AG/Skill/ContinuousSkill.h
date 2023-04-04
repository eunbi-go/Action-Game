// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SkillActor.h"
#include "ContinuousSkill.generated.h"

/**
 * 
 */
UCLASS()
class AG_API AContinuousSkill : public ASkillActor
{
	GENERATED_BODY()
	
public:
	AContinuousSkill();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void Finish(UNiagaraComponent* comp);


public:
	void SetNiagaraOnOff(bool _value) { mIsNiagaraActive = _value; }
	void Remove() { mNiagara->GetAsset()->FinishDestroy(); }
	
protected:
	bool mIsNiagaraActive;

public:
	FRotator rot;
};
