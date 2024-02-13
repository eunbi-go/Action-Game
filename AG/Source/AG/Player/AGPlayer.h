// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "GameFramework/Character.h"
#include "../Interface/HitInterface.h"
#include "../CharacterBase/AGBaseCharacter.h"
#include "AGPlayer.generated.h"

class UCharacterStatComponent;

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedWeapon UMETA(DisplayName = "Equipped One-HandWeapon"),
};

UENUM(BlueprintType)
enum class ESkillState : uint8
{
	ESS_None UMETA(DisplayName = "None"),
	ESS_Sprint UMETA(DisplayName = "Sprint"),
	ESS_Ribbon UMETA(DisplayName = "Ribbon"),
	ESS_Slash UMETA(DisplayName = "Slash"),
	ESS_HardAttack UMETA(DisplayName = "HardAttack"),
};

UCLASS()
class AG_API AAGPlayer : public AAGBaseCharacter
{
	GENERATED_BODY()

public:
	AAGPlayer();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	

	bool AddItem(EITEM_ID _itemID);
	EITEM_ID SelectItem();
	UCharacterStatComponent* GetStat()
	{
		return mStat;
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	UCameraComponent* mCameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
		USpringArmComponent* mSpringArmComp;

protected:
	virtual void BeginPlay() override;

	//-----------
	// Input 
	//-----------
	void MoveForward(float _value);
	void MoveHorizontal(float _value);
	void Turn(float _value);
	void LookUp(float _value);
	virtual void JumpKey();
	virtual void CrouchKey();
	virtual void GuardKey();

	virtual void EquipWeaponKey();
	virtual void NormalAttackKey();
	void InventoryOnOffKey();
	void Item1Key();
	void Item2Key();
	void Item3Key();
	void Item4Key();
	void Item5Key();
	void DashKey();

	virtual void PlayMontage(FName _montageName, FName _sectionName = "");
	virtual void SpawnEffect();

	



	UPROPERTY(VisibleAnywhere, Category = Component, meta = (AllowPrivateAccess = true))
	class UValkyrieAnimInstance* mAnimInst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montages", meta = (AllowPrivateAccess = true))
	TMap<FName, UAnimMontage*>	mMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	UCharacterStatComponent* mStat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FName			mPlayerTableRowName;

	float mDirection = 0.f;

	ECharacterState mCharacterState;
	EActionState	mActionState;
	ESkillState		mSkillState;
	
		UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack")
	bool mIsAttacking;

	UPROPERTY()
	FTimerHandle	mTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	UNiagaraComponent* mDash;

	bool mIsGuard = false;

public:
	float GetDirection() { return mDirection; }
	ECharacterState GetCharacterState() { return mCharacterState; }
	bool GetIsGuard() { return mIsGuard; }
};
