#pragma once

#include "../BasicInfo.h"
#include "GameFramework/Actor.h"
#include "CollisionActor.generated.h"

UENUM()
enum class ECollisionType : uint8
{
	ECS_Capsule,
	ECS_Box
};

UENUM()
enum class EHitType : uint8
{
	EHT_Once,
	EHT_Continuous
};

UCLASS()
class AG_API ACollisionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACollisionActor();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnCapsuleOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCapsuleOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void ClearTimer();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		UBoxComponent* mCollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		UCapsuleComponent* mCollisionCapsule;

protected:
	virtual void BeginPlay() override;

private:
	void GetHit(AActor* _hitActor);

public:
	void SetCollisionShape(ECollisionType _value)
	{
		mCollisionShape = _value;

		if (mCollisionShape == ECollisionType::ECS_Capsule)
		{
		}
		else
		{
		}
	}

	void SetCollisionProfileName(FName _value)
	{
		mCollisionProfileName = _value;

		if (mCollisionShape == ECollisionType::ECS_Capsule)
			mCollisionCapsule->SetCollisionProfileName(mCollisionProfileName);
		else if (mCollisionShape == ECollisionType::ECS_Box)
			mCollisionBox->SetCollisionProfileName(mCollisionProfileName);
	}

	void SetCapsuleHalfHeight(float _value)
	{
		mCollisionCapsule->SetCapsuleHalfHeight(_value);
	}
	void SetCapsuleRadius(float _value)
	{
		mCollisionCapsule->SetCapsuleRadius(_value);
	}

	void SetBoxExtent(const FVector& _value)
	{
		mCollisionBox->SetBoxExtent(_value);
	}

	void SetHitType(EHitType _value)
	{
		mHitType = _value;
	}

	void SetHitSeconds(float _value)
	{
		mHitSeconds = _value;
	}

	void SetOnColl(bool _value)
	{
		onColl = _value;
	}

private:
	ECollisionType	mCollisionShape;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EHitType	mHitType;

	float	mHitSeconds;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USceneComponent* mRoot;



	FName	mCollisionProfileName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TArray<AActor*> mActorsToIgnoreArray;

	FTimerHandle mTimer;

	bool onColl = false;
};
