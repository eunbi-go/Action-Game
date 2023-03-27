// Fill out your copyright notice in the Description page of Project Settings.

//---------------------------
// ���� Ŭ����
//---------------------------

#pragma once

#include "EngineMinimal.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "UObject/NoExportTypes.h"
#include "BasicInfo.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(AG, Log, All);

// �α� ����� ���� ��ũ�� ����: �Լ��̸�[�ٹ�ȣ]
//  __FUNCTION__: �Լ� �̸��� �����´�.
//  __LINE__: �ش� ���Ͽ����� �� ���� ������ ���´�.
// FString::FromInt(__LINE__): �� ��ȣ�� ������ ���ͼ� ���ڿ��� ������ش�.
#define	LOG_CALLINFO	(FString(__FUNCTION__) + TEXT("[") + FString::FromInt(__LINE__) + TEXT("]"))

// UE_LOG: �𸮾󿡼� �α� ����� ���� ���.
//  ...: �������ڸ� ����ϱ� ����. (������ ������ ���ϴ´�� �߰� ����)
//  %s: ���ڿ��� �޾ƿͼ� ��ü���ֱ� ���� ����Ѵ�. 
//		���ڿ� ������ �����ؾ� ��.
//		-> FString �տ� * �� �ٿ��� �������� �ϸ� FString�� ������ �ִ� ���ڿ� �����͸� ���´�,
//  __VA_ARGS__ : �������ڸ� ���´�.
#define	LOG(Format, ...)	UE_LOG(MYUE, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define	LOGSTRING(Str)		UE_LOG(MYUE, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *Str)


// �������� ����Ʈ�� �α׸� ����ش�.
void PrintViewport(float Time, const FColor& Color, const FString& Text);



/**
 * 
 */
UCLASS()
class AG_API UBasicInfo : public UObject
{
	GENERATED_BODY()
	
};
