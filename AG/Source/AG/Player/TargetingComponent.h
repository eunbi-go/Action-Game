// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "Components/ActorComponent.h"
#include "TargetingComponent.generated.h"


class AMonster;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AG_API UTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTargetingComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void CheckTarget();
	void SetTargetLock();

	void SetOwner(ACharacter* _value)
	{
		if (_value)
			mOwner = _value;
	}

protected:
	virtual void BeginPlay() override;

private:
	void LockingCamera(float DeltaTime);
	AActor* GetClosetActor(TArray<AActor*> actors);





public:	

	// 해당 Component 를 멤버변수로 갖고 있는 클래스
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Targeting", meta = (AllowPrivateAccess = true))
	ACharacter* mOwner;

	// 타겟이 될 객체
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Targeting", meta = (AllowPrivateAccess = true))
	AMonster* mTarget;

	// 타겟이 세팅되어 있다: true
	// 타겟이 없다:		  false
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Targeting", meta = (AllowPrivateAccess = true))
	bool mIsTargetLock = false;

	// Target 을 감지하기 위한 Sphere 의 반지름
	float mSphereRadius = 1000.f;
};