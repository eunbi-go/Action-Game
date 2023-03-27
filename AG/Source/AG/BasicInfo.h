// Fill out your copyright notice in the Description page of Project Settings.

//---------------------------
// 공용 클래스
//---------------------------

#pragma once

#include "EngineMinimal.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "UObject/NoExportTypes.h"
#include "BasicInfo.generated.h"


DECLARE_LOG_CATEGORY_EXTERN(AG, Log, All);

// 로그 사용을 위해 매크로 생성: 함수이름[줄번호]
//  __FUNCTION__: 함수 이름을 가져온다.
//  __LINE__: 해당 파일에서의 줄 수를 정수로 얻어온다.
// FString::FromInt(__LINE__): 줄 번호를 정수로 얻어와서 문자열로 만들어준다.
#define	LOG_CALLINFO	(FString(__FUNCTION__) + TEXT("[") + FString::FromInt(__LINE__) + TEXT("]"))

// UE_LOG: 언리얼에서 로그 출력을 위해 사용.
//  ...: 가변인자를 사용하기 위해. (인자의 개수를 원하는대로 추가 가능)
//  %s: 문자열을 받아와서 대체해주기 위해 사용한다. 
//		문자열 포인터 지정해야 함.
//		-> FString 앞에 * 을 붙여서 역참조를 하면 FString이 가지고 있는 문자열 포인터를 얻어온다,
//  __VA_ARGS__ : 가변인자를 얻어온다.
#define	LOG(Format, ...)	UE_LOG(MYUE, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define	LOGSTRING(Str)		UE_LOG(MYUE, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *Str)


// 에디터의 뷰포트에 로그를 띄워준다.
void PrintViewport(float Time, const FColor& Color, const FString& Text);



/**
 * 
 */
UCLASS()
class AG_API UBasicInfo : public UObject
{
	GENERATED_BODY()
	
};
