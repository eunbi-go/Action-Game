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

	// �ش� Component �� ��������� ���� �ִ� Ŭ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Targeting", meta = (AllowPrivateAccess = true))
	ACharacter* mOwner;

	// Ÿ���� �� ��ü
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Targeting", meta = (AllowPrivateAccess = true))
	AMonster* mTarget;

	// Ÿ���� ���õǾ� �ִ�: true
	// Ÿ���� ����:		  false
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Targeting", meta = (AllowPrivateAccess = true))
	bool mIsTargetLock = false;

	// Target �� �����ϱ� ���� Sphere �� ������
	float mSphereRadius = 1000.f;
};