// Fill out your copyright notice in the Description page of Project Settings.


#include "NearestTargetData.h"
#include "AbilitySystemComponent.h"
#include "../../Monster/Monster.h"

UNearestTargetData* UNearestTargetData::CreateNearestTargetData(UGameplayAbility* OwningAbility)
{
	UNearestTargetData* obj = NewAbilityTask<UNearestTargetData>(OwningAbility);
	return obj;
}

void UNearestTargetData::Activate()
{
	Super::Activate();

	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		// 현재 클라이언트에 있고 로컬로 제어되는 경우 델리게이트 브로드캐스트 + 서버로 데이터 전송
		SendTargetData();
	}
	else
	{
		const FGameplayAbilitySpecHandle specHandle = GetAbilitySpecHandle();
		const FPredictionKey activationPredictionKey = GetActivationPredictionKey();

		// 현재 서버에 있음!
		// -> Target Data를 받는다.
		// 서버에서 Activate() 가 호출되자마자 서버는 콜백을 델리게이트에 바인딩한다.
		AbilitySystemComponent->AbilityTargetDataSetDelegate(specHandle,
			activationPredictionKey).AddUObject(this, &UNearestTargetData::OnTargetDataReplicatedCallback);

		// 너무 늦어서 Target Data가 이미 전송되었고 델리게이트가 이미 브로드캐스트된 경우
		// -> 해당 Target Data의 델리게이트를 확인하거나 호출한다.

		// 델리게이트가 이미 설정되었는가?
		const bool isCalledDelegate = AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(specHandle, activationPredictionKey);
		if (!isCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UNearestTargetData::SendTargetData()
{
	FScopedPredictionWindow scopedPrediction(AbilitySystemComponent);

	FHitResult hit;
	TWeakObjectPtr<AActor> owner = Ability->GetCurrentActorInfo()->AvatarActor;


	TArray<FOverlapResult>	resultArray;
	FCollisionQueryParams	param(NAME_None, false, owner.Get()); // 무시할 액터

	
	bool isCollision = GetWorld()->OverlapMultiByChannel(
		resultArray,
		owner->GetActorLocation(),
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel12,
		FCollisionShape::MakeBox(FVector(90000.f)),
		param);

	AMonster* target = nullptr;
	float distance = 10000000.f;
	int i = 0, cnt = resultArray.Num();
	FVector location = owner->GetActorLocation();
	for (i = 0; i < cnt; ++i)
	{
		AMonster* monster = Cast<AMonster>(resultArray[i].GetActor());
		if (IsValid(monster))
		{
			float distanceToMonster = fabsf((monster->GetActorLocation() - location).Size());
			if (distanceToMonster < distance)
			{
				distance = distanceToMonster;
				target = monster;
				hit.Location = monster->GetActorLocation();
			}
		}
	}

	//PrintViewport(3.f, FColor::Red, FString::Printf(TEXT("%f"), distance));

	FGameplayAbilityTargetDataHandle dataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* data = new FGameplayAbilityTargetData_SingleTargetHit();
	data->HitResult = hit;
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
}

void UNearestTargetData::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	// Target Data 수신할 때 호출된다.
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		mValidData.Broadcast(DataHandle);
	}
}
