// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "Components/ActorComponent.h"
#include "TargetingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AG_API UTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTargetingComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void CheckTarget();
	AActor* GetClosetActor(TArray<AActor*> actors, FName targetTag);

	void SetTargetLock()
	{
		mIsTargetLock = !mIsTargetLock;

		if (mIsTargetLock)
		{
			CheckTarget();
		}
		else
		{
			mOwner->bUseControllerRotationYaw = false;
			mOwner->GetCharacterMovement()->bOrientRotationToMovement = true;
			mOwner->SetActorTickEnabled(true);
		}
	}

	void SetOwner(ACharacter* _value)
	{
		if (_value)
			mOwner = _value;
	}

protected:
	virtual void BeginPlay() override;

private:
	void LockingCamera(float DeltaTime);

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	ACharacter* mOwner;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	ACharacter* mTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	bool mIsTargetLock = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	bool mIsYaw = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	bool mIsControl = false;
};
