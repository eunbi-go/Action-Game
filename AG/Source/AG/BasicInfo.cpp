// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicInfo.h"

DEFINE_LOG_CATEGORY(AG)

void PrintViewport(float Time, const FColor& Color, const FString& Text)
{
	GEngine->AddOnScreenDebugMessage(-1, Time, Color, Text);
}