// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AGAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class AG_API UAGAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UAGAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 복제 지정자 ReplicatedUsing: 복제 변수로 만든다.
	// -> 클라에서 서버로 변경된 값을 알린다.
	// -> 서버에서 변경되면 클라이언트는 업데이트된 값을 받게 된다.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Hp)
	FGameplayAttributeData mHp;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, mHp);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHp)
	FGameplayAttributeData mMaxHp;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, mMaxHp);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mp)
		FGameplayAttributeData mMp;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, mMp);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMp)
		FGameplayAttributeData mMaxMp;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, mMaxMp);

	UFUNCTION()
	void OnRep_Hp(const FGameplayAttributeData& preHp) const;

	UFUNCTION()
	void OnRep_MaxHp(const FGameplayAttributeData& prMaxHp) const;

	UFUNCTION()
	void OnRep_Mp(const FGameplayAttributeData& preMp) const;

	UFUNCTION()
	void OnRep_MaxMp(const FGameplayAttributeData& prMaxMp) const;
};
