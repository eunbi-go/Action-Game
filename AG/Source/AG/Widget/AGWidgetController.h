// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "UObject/NoExportTypes.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AGWidgetController.generated.h"

/**
 * 
 * 위젯 컨트롤러가 데이터를 브로드캐스트하면 위젯이 해당 데이터를 수신하고 이에 응답한다.
 * 위젯 컨트롤러는 자신이 어떤 위젯과 연관되어 있는지 알지 못하지만,
 * 위젯 자체는 해당 컨트롤러가 무엇인지 알 수 있다.
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
