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

DECLARE_DELEGATE_RetVal(FGameplayAttribute, FAttributeSignature);

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
	 * Attribute가 변경되기 전 호출된다.
	 * Epic에서는 이 함수를 Clamp 용도로 사용하는 것을 권장함. 
	 */
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	/**
	 * Attribute가 변경된 후, 호출된다.
	 */
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;



	//TMap<FGameplayTag, TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr> mTagsToAttributes;
	TMap<FGameplayTag, FAttributeSignature> mTagsToAttributes;

	//TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr mFunctionPointer;


	// 복제 지정자 ReplicatedUsing: 복제 변수로 만든다.
	// -> 클라에서 서버로 변경된 값을 알린다.
	// -> 서버에서 변경되면 클라이언트는 업데이트된 값을 받게 된다.
	UPROPERTY(BlueprintReadOnly, Category = "Vital", ReplicatedUsing = OnRep_Hp)
	FGameplayAttributeData mHp;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, mHp);

	UPROPERTY(BlueprintReadOnly, Category = "Vital", ReplicatedUsing = OnRep_MaxHp)
	FGameplayAttributeData mMaxHp;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, mMaxHp);

	UPROPERTY(BlueprintReadOnly, Category = "Vital", ReplicatedUsing = OnRep_Mp)
	FGameplayAttributeData mMp;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, mMp);

	UPROPERTY(BlueprintReadOnly, Category = "Vital", ReplicatedUsing = OnRep_MaxMp)
	FGameplayAttributeData mMaxMp;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, mMaxMp);

	UPROPERTY(BlueprintReadOnly, Category = "Vital", ReplicatedUsing = OnRep_Coin)
	FGameplayAttributeData mCoin;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, mCoin);

	/**
	 * First Attributes 
	 **/

	UPROPERTY(BlueprintReadOnly, Category = "First Attributes", ReplicatedUsing = OnRep_Strength)
	FGameplayAttributeData mStrength;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, mStrength);

	UPROPERTY(BlueprintReadOnly, Category = "First Attributes", ReplicatedUsing = OnRep_Intelligence)
	FGameplayAttributeData mIntelligence;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, mIntelligence);

	UPROPERTY(BlueprintReadOnly, Category = "First Attributes", ReplicatedUsing = OnRep_Resilience)
	FGameplayAttributeData mResilience;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, mResilience);

	UPROPERTY(BlueprintReadOnly, Category = "First Attributes", ReplicatedUsing = OnRep_Dexterity)
	FGameplayAttributeData mDexterity;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, mDexterity);


	/**
	 * Second Attributes
	 **/

	UPROPERTY(BlueprintReadOnly, Category = "Second Attributes", ReplicatedUsing = OnRep_Armor)
	FGameplayAttributeData mArmor;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, mArmor);

	UPROPERTY(BlueprintReadOnly, Category = "Second Attributes", ReplicatedUsing = OnRep_BlockPercent)
	FGameplayAttributeData mBlockPercent;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, mBlockPercent);

	UPROPERTY(BlueprintReadOnly, Category = "Second Attributes", ReplicatedUsing = OnRep_CriticalPercent)
	FGameplayAttributeData mCriticalPercent;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, mCriticalPercent);

	UPROPERTY(BlueprintReadOnly, Category = "Second Attributes", ReplicatedUsing = OnRep_CriticalDamage)
	FGameplayAttributeData mCriticalDamage;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, mCriticalDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Second Attributes", ReplicatedUsing = OnRep_CriticalResistance)
	FGameplayAttributeData mCriticalResistance;
	ATTRIBUTE_ACCESSORS(UAGAttributeSet, mCriticalResistance);







	UFUNCTION()
	void OnRep_Hp(const FGameplayAttributeData& preHp) const;

	UFUNCTION()
	void OnRep_MaxHp(const FGameplayAttributeData& prMaxHp) const;

	UFUNCTION()
	void OnRep_Mp(const FGameplayAttributeData& preMp) const;

	UFUNCTION()
	void OnRep_MaxMp(const FGameplayAttributeData& prMaxMp) const;

	UFUNCTION()
	void OnRep_Coin(const FGameplayAttributeData& preCoin) const;



	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& PreStrength) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& preIntelligence) const;

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& PreResilience) const;

	UFUNCTION()
	void OnRep_Dexterity(const FGameplayAttributeData& PreDexterity) const;

	

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& PreArmor) const;

	UFUNCTION()
	void OnRep_BlockPercent(const FGameplayAttributeData& PreBlockPercent) const;

	UFUNCTION()
	void OnRep_CriticalPercent(const FGameplayAttributeData& PreCriticalPercent) const;

	UFUNCTION()
	void OnRep_CriticalDamage(const FGameplayAttributeData& PreCriticalDamage) const;

	UFUNCTION()
	void OnRep_CriticalResistance(const FGameplayAttributeData& PreCriticalResistance) const;

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
};
