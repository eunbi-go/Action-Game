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
		// ���� Ŭ���̾�Ʈ�� �ְ� ���÷� ����Ǵ� ��� ��������Ʈ ��ε�ĳ��Ʈ + ������ ������ ����
		SendTargetData();
	}
	else
	{
		const FGameplayAbilitySpecHandle specHandle = GetAbilitySpecHandle();
		const FPredictionKey activationPredictionKey = GetActivationPredictionKey();

		// ���� ������ ����!
		// -> Target Data�� �޴´�.
		// �������� Activate() �� ȣ����ڸ��� ������ �ݹ��� ��������Ʈ�� ���ε��Ѵ�.
		AbilitySystemComponent->AbilityTargetDataSetDelegate(specHandle,
			activationPredictionKey).AddUObject(this, &UNearestTargetData::OnTargetDataReplicatedCallback);

		// �ʹ� �ʾ Target Data�� �̹� ���۵Ǿ��� ��������Ʈ�� �̹� ��ε�ĳ��Ʈ�� ���
		// -> �ش� Target Data�� ��������Ʈ�� Ȯ���ϰų� ȣ���Ѵ�.

		// ��������Ʈ�� �̹� �����Ǿ��°�?
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
	FCollisionQueryParams	param(NAME_None, false, owner.Get()); // ������ ����

	
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
	// Target Data ������ �� ȣ��ȴ�.
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		mValidData.Broadcast(DataHandle);
	}
}
