// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "GameFramework/Actor.h"
#include "../AbilitySystem/AGAbilitySystemComponent.h"
#include "AGEffectActor.generated.h"

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovePolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotRemove
};

class UAbilitySystemComponent;


UCLASS()
class AG_API AAGEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAGEffectActor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* target, TSubclassOf<UGameplayEffect> gmaePlayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnBeginOverlap(AActor* targetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* targetActor);


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	bool	mIsDestroyOnEffectRemoval = false;
	

	// 즉각적인 효과
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> mInstantGameplayEffectClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy	mInstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	// 지속 효과
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> mDurationGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy	mDurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	// 무한 효과 : 사용자가 직접 해제해줘야 함.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> mInfiniteGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy	mInfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectRemovePolicy	mInfiniteEffectRemovePolicy = EEffectRemovePolicy::RemoveOnEndOverlap;

	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> mActiveEffectHandles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	float mActorLevel = 1.f;
};
