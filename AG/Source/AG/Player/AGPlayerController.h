// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AGPlayerController.generated.h"

class IEnemyInterface;
class UAGAbilitySystemComponent;
class UAGInputConfig;
class UInputMappingContext;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRangeReleaseDelegate, const float&, Time);

/**
 * 
 */
UCLASS()
class AG_API AAGPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAGPlayerController();
	virtual void PlayerTick(float DeltaTime) override;
	FOnRangeReleaseDelegate	mOnRangeRelease;

protected:
	virtual void BeginPlay() override;
	// 컴포넌트들이 모두 초기화된 다음 호출되는 함수
	virtual void PostInitializeComponents() override;
	// 플레이어 컨트롤러가 Pawn에 빙의될 때 호출
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;
	virtual void SetupInputComponent() override;

public:
	virtual void Tick(float DeltaTime) override;


	void SetInputModeType(INPUT_MODE_TYPE _type);
	UAGAbilitySystemComponent* GetASC();

private:
	UPROPERTY()
	UAGAbilitySystemComponent* mAGasc;



	//------------------------
	// Inputs
	//------------------------
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UAGInputConfig* mInputConfig;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* mInputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input|Actions")
	UInputAction* mInputAction_W;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Actions")
	UInputAction* mInputAction_A;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Actions")
	UInputAction* mInputAction_S;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Actions")
	UInputAction* mInputAction_D;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Actions")
	UInputAction* mInputAction_Q;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Actions")
	UInputAction* mInputAction_E;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Actions")
	UInputAction* mInputAction_R;

	UPROPERTY(EditDefaultsOnly, Category = "Input|Actions")
	UInputAction* mInputAction_T;
	
public:
	bool mIsRangeReleased = false;
	float mPressingTime = 0.f;
	bool mIsCheckPressingTime = false;
};
