// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../BasicInfo.h"
#include "GameFramework/HUD.h"
#include "AGHUD.generated.h"

/**
 * 
 */

class UMainWidget;
class UMainWidgetController;
struct FWidgetControllerParams;
class UAbilitySystemComponent;
class UAttributeSet;
class UAttributeWidgetController;
class UStatWidget;

UCLASS()
class AG_API AAGHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AAGHUD();

	UPROPERTY();
	UMainWidget* mMainWidget;

	UMainWidgetController* GetMainWidgetController(const FWidgetControllerParams& params);
	UAttributeWidgetController* GetAttributeWidgetController(const FWidgetControllerParams& params);

	void InitMainWidget(APlayerController* pc, APlayerState* ps, UAbilitySystemComponent* asc, UAttributeSet* as);


protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMainWidget> mMainWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMainWidgetController> mMainWidgetControllerClass;
	
	UPROPERTY()
	UMainWidgetController* mMainWidgetController;

	///////////
	UPROPERTY(EditAnywhere)
	TSubclassOf<UStatWidget> mStatWidgetClass;

	UPROPERTY()
	UAttributeWidgetController* mAttributeWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeWidgetController> mAttributeWidgetControllerClass;
	
};
