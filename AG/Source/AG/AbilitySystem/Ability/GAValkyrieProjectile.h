// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "../Ability/AGGameplayAbility.h"
#include "GAValkyrieProjectile.generated.h"

class AParticleNiagara;

DECLARE_MULTICAST_DELEGATE(FOnProjectileStart)
/**
 * 
 */

UCLASS()
class AG_API UGAValkyrieProjectile : public UAGGameplayAbility
{
	GENERATED_BODY()
	
public:
	FOnProjectileStart mOnProjectileStart;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category="Projectile")
	void SpawnProjectile(const float& Time);

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void Pause();
	
	UFUNCTION()
	void SpawnEffect();

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void AbilityEnd();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AParticleNiagara> mProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> mDamageEffectClass;

	bool mIsSpawnEffect = false;

	UPROPERTY()
	FTimerHandle mTimer;

	uint8 mSpawnCount = 0;

	UPROPERTY()
	TArray<FVector> mLocationArray;

	UPROPERTY()
	TArray<FVector> mDirectionArray;

	UPROPERTY(EditDefaultsOnly)
	float mOffset = 400.f;
};
