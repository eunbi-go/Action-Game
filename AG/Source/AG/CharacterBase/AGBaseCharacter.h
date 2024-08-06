// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "AbilitySystemInterface.h"
#include "../Interface/CombatInterface.h"
#include "GameFramework/Character.h"
#include "AGBaseCharacter.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;

UCLASS(Abstract)	// 추상 클래스로 만든다.
class AG_API AAGBaseCharacter : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAGBaseCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return mAttributeSet; }

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();

	/*UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> mWeapon;*/

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> mAbilitySystemComp;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> mAttributeSet;

	// init AGAttributes by GameplayEffect
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> mDefaultFirstAttributes;

	// mDefaultFirstAttributes에 의존해서 변화되는 Attributes
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> mDefaultSecondAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> mDefaultVitalAttributes;

	// init AGAttributes by GameplayEffect
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	void InitializeDefaultAttributes() const;




	UPROPERTY(EditAnywhere, Category = "GAS|Ability")
	TArray<TSubclassOf<UGameplayAbility>> mStartupAbilites;


	void AddCharacterAbilities();
	UFUNCTION(BlueprintCallable)
	void TryActivateAbility(int32 Id);
};
