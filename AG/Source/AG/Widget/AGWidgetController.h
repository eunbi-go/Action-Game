// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "UObject/NoExportTypes.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AGWidgetController.generated.h"

/**
 * 
 * ���� ��Ʈ�ѷ��� �����͸� ��ε�ĳ��Ʈ�ϸ� ������ �ش� �����͸� �����ϰ� �̿� �����Ѵ�.
 * ���� ��Ʈ�ѷ��� �ڽ��� � ������ �����Ǿ� �ִ��� ���� ��������,
 * ���� ��ü�� �ش� ��Ʈ�ѷ��� �������� �� �� �ִ�.
 */
USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() { }
	FWidgetControllerParams(APlayerController* pc, APlayerState* ps, UAbilitySystemComponent* asc, UAttributeSet* as)
		: playerController(pc), playerState(ps), abilitySystemComp(asc), attributeSet(as) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlayerController* playerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		APlayerState* playerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAbilitySystemComponent* abilitySystemComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAttributeSet* attributeSet = nullptr;
};


UCLASS()
class AG_API UAGWidgetController : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& params);

	virtual void BroadcastInitValues();
	virtual void BindCallbacksToDependecies();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	APlayerController* mPlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	APlayerState* mPlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	UAbilitySystemComponent* mAbilitySystemComp;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	UAttributeSet* mAttributeSet;
};
