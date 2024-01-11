// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "AGBaseCharacter.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)	// 추상 클래스로 만든다.
class AG_API AAGBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAGBaseCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return mAttributeSet; }

protected:
	virtual void BeginPlay() override;

	/*UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> mWeapon;*/

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> mAbilitySystemComp;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> mAttributeSet;
};
