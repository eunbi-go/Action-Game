// Fill out your copyright notice in the Description page of Project Settings.


#include "AGWidgetController.h"

void UAGWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& params)
{
	mPlayerController = params.playerController;
	mPlayerState = params.playerState;
	mAbilitySystemComp = params.abilitySystemComp;
	mAttributeSet = params.attributeSet;
}

void UAGWidgetController::BroadcastInitValues()
{
}
