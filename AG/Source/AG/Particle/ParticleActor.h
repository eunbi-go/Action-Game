// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "GameFramework/Actor.h"
#include "ParticleActor.generated.h"

UCLASS()
class AG_API AParticleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AParticleActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


public:
	void SetSound(const FString& _path, bool _isPlay = true);
	void SetSound(USoundBase* _sound, bool _isPlay = true);


public:
	virtual void SetParticle(UParticleSystem* _particle);
	virtual void SetParticle(UNiagaraSystem* _particle);
	virtual void SetParticle(const FString& _path);


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UAudioComponent* mAudio;
};
