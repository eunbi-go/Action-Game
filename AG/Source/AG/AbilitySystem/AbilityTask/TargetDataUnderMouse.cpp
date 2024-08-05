// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetDataUnderMouse.h"
#include "AbilitySystemComponent.h"

// 비동기식 블루프린트 노드
// -> 여러 출력 실행 핀을 가질 수 있다.
// -> 여러 출력 핀을 생성하는 방법 : 델리게이트를 브로드캐스트하는 것
// -> 모든 출력 실행 핀은 이 클래스의 델리게잍 변수로 존재해야 한다.
UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* obj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return obj;
}

void UTargetDataUnderMouse::Activate()
{
	// 현재 클라이언트에 있고 로컬로 제어되는 경우 델리게이트 브로드캐스트 + 서버로 데이터 전송

	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		const FGameplayAbilitySpecHandle specHandle = GetAbilitySpecHandle();
		const FPredictionKey activationPredictionKey = GetActivationPredictionKey();

		// 현재 서버에 있음!
		// -> Target Data를 받는다.
		// 서버에서 Activate() 가 호출되자마자 서버는 콜백을 델리게이트에 바인딩한다.
		AbilitySystemComponent->AbilityTargetDataSetDelegate(specHandle, 
			activationPredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
	
		// 너무 늦어서 Target Data가 이미 전송되었고 델리게이트가 이미 브로드캐스트된 경우
		// -> 해당 Target Data의 델리게이트를 확인하거나 호출한다.

		// if : 델리게이트가 이미 설정되었는가?
		const bool isCalledDelegate = AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(specHandle, activationPredictionKey);
		if (!isCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}


}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	FScopedPredictionWindow scopedPrediction(AbilitySystemComponent);

	APlayerController* pc = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult cursorHit;
	pc->GetHitResultUnderCursor(ECC_Visibility, false, cursorHit);
	
	FGameplayAbilityTargetDataHandle dataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* data = new FGameplayAbilityTargetData_SingleTargetHit();
	data->HitResult = cursorHit;
	dataHandle.Add(data);

	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		dataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey
	);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		mValidData.Broadcast(dataHandle);
	}

	//// Ability를 활성화하자마자 유효한 데이터를 갖는 델리게이트를 브로드캐스트한다.
	//// 하지만 비동기 작업이기 때문에 당장 이루어질 필요는 없다.
	//// 따라서 일부 Ability Task에 대한 게임플레이 메커니즘에 따라 델리게이트를 나중에 브로드캐스트하길 원할 수 있다.
	//// 하지만 지금은 바로 브로드캐스트한다.
	//mValidData.Broadcast(cursorHit.Location);
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	// Target Data 수신할 때 호출된다.
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		mValidData.Broadcast(DataHandle);
	}
}
