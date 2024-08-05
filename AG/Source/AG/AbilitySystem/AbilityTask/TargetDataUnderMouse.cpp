// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetDataUnderMouse.h"
#include "AbilitySystemComponent.h"

// �񵿱�� �������Ʈ ���
// -> ���� ��� ���� ���� ���� �� �ִ�.
// -> ���� ��� ���� �����ϴ� ��� : ��������Ʈ�� ��ε�ĳ��Ʈ�ϴ� ��
// -> ��� ��� ���� ���� �� Ŭ������ �����ԟ� ������ �����ؾ� �Ѵ�.
UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* obj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return obj;
}

void UTargetDataUnderMouse::Activate()
{
	// ���� Ŭ���̾�Ʈ�� �ְ� ���÷� ����Ǵ� ��� ��������Ʈ ��ε�ĳ��Ʈ + ������ ������ ����

	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		const FGameplayAbilitySpecHandle specHandle = GetAbilitySpecHandle();
		const FPredictionKey activationPredictionKey = GetActivationPredictionKey();

		// ���� ������ ����!
		// -> Target Data�� �޴´�.
		// �������� Activate() �� ȣ����ڸ��� ������ �ݹ��� ��������Ʈ�� ���ε��Ѵ�.
		AbilitySystemComponent->AbilityTargetDataSetDelegate(specHandle, 
			activationPredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
	
		// �ʹ� �ʾ Target Data�� �̹� ���۵Ǿ��� ��������Ʈ�� �̹� ��ε�ĳ��Ʈ�� ���
		// -> �ش� Target Data�� ��������Ʈ�� Ȯ���ϰų� ȣ���Ѵ�.

		// if : ��������Ʈ�� �̹� �����Ǿ��°�?
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

	//// Ability�� Ȱ��ȭ���ڸ��� ��ȿ�� �����͸� ���� ��������Ʈ�� ��ε�ĳ��Ʈ�Ѵ�.
	//// ������ �񵿱� �۾��̱� ������ ���� �̷���� �ʿ�� ����.
	//// ���� �Ϻ� Ability Task�� ���� �����÷��� ��Ŀ���� ���� ��������Ʈ�� ���߿� ��ε�ĳ��Ʈ�ϱ� ���� �� �ִ�.
	//// ������ ������ �ٷ� ��ε�ĳ��Ʈ�Ѵ�.
	//mValidData.Broadcast(cursorHit.Location);
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	// Target Data ������ �� ȣ��ȴ�.
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		mValidData.Broadcast(DataHandle);
	}
}
