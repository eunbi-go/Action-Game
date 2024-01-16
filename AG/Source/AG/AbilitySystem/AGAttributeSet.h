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

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() {}

	FGameplayEffectContextHandle effectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* sourceASC = nullptr;

	UPROPERTY()
	AActor* sourceAvatarActor = nullptr;

	UPROPERTY()
	AController* sourceController = nullptr;

	UPROPERTY()
	ACharacter* sourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* targetASC = nullptr;

	UPROPERTY()
	AActor* targetAvatarActor = nullptr;

	UPROPERTY()
	AController* targetController = nullptr;

	UPROPERTY()
	ACharacter* targetCharacter = nullptr;
};

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

	/**
	 * Attribute�� ����Ǳ� �� ȣ��ȴ�.
	 * Epic������ �� �Լ��� Clamp �뵵�� ����ϴ� ���� ������. 
	 */
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	/**
	 * Attribute�� ����� ��, ȣ��ȴ�.
	 */
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// ���� ������ ReplicatedUsing: ���� ������ �����.
	// -> Ŭ�󿡼� ������ ����� ���� �˸���.
	// -> �������� ����Ǹ� Ŭ���̾�Ʈ�� ������Ʈ�� ���� �ް� �ȴ�.
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

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Coin)
	FGameplayAttributeData mCoin;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, mCoin);

	/**
	 * Combat 
	 **/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Defense)
	FGameplayAttributeData mDefense;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, mDefense);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Attack)
	FGameplayAttributeData mAttack;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, mAttack);

	UFUNCTION()
	void OnRep_Hp(const FGameplayAttributeData& preHp) const;

	UFUNCTION()
	void OnRep_MaxHp(const FGameplayAttributeData& prMaxHp) const;

	UFUNCTION()
	void OnRep_Mp(const FGameplayAttributeData& preMp) const;

	UFUNCTION()
	void OnRep_MaxMp(const FGameplayAttributeData& prMaxMp) const;

	UFUNCTION()
	void OnRep_Defense(const FGameplayAttributeData& preDefense) const;

	UFUNCTION()
	void OnRep_Attack(const FGameplayAttributeData& preAttack) const;

	UFUNCTION()
	void OnRep_Coin(const FGameplayAttributeData& preCoin) const;


private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
};
