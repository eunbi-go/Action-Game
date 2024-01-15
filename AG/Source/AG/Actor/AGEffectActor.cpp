// Fill out your copyright notice in the Description page of Project Settings.


#include "AGEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "../AbilitySystem/AGAttributeSet.h"

AAGEffectActor::AAGEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAGEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAGEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// ���� Ŭ������ GameplayEffect�� �����Ǿ� ������ target�� GameplayEffect�� �����Ѵ�.
void AAGEffectActor::ApplyEffectToTarget(AActor* target, TSubclassOf<UGameplayEffect> gmaePlayEffectClass)
{
	/*
		IAbilitySystemInterface* targetASC = Cast<IAbilitySystemInterface>(target)
		���� ��� ��� �Ʒ��� ���� ���� �Լ� ���̺귯�� ���.
		GetAbilitySystemComponent() �Լ� �ȿ� ���� �ڵ尡 �������.
		��� ����� �������̽��� �����ϴ� ���� �ƴ�. 
		GAS�� �̷��� �ش����� ��츦 ó���ϵ��� ����� ���� ���̺귯���� �ְ�, 
		�װ��� UAbilitySystemBlueprintLibrary
	*/
	UAbilitySystemComponent* targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(target);
	if (targetASC == nullptr)
		return;
	check(gmaePlayEffectClass);

	/*
		�����÷��� ȿ���� ȿ�� ���ؽ�Ʈ�� ������ ������ �ִ�. 
		�����÷��� ȿ���� �̸� �ѷ��� ���ؽ�Ʈ�� �����ϴ� ������� ���� ����.(ȿ���� ����Ű�� ���ΰ� ���, � ȿ������ ���)
		�����÷��� ȿ�� ���ؽ�Ʈ�� ���� �÷��� ȿ��, Ư�� ȿ�� ���ؽ�Ʈ�� ������ �����Ͽ� ȿ�� ���� ������Ų��.
		������ ȿ�� ���ؽ�Ʈ�� �ڵ��� ó���Ѵ�. ��, �ڵ��� ���ؽ�Ʈ ��ü�� ���� �����͸� �����ϴ� ���̴�.

		Q. ���ؽ�Ʈ �ڵ��� ��� ����°�
		GAS���� ���� �� �ִ�. 
		MakeEffectContext() : ȿ�� ���ؽ�Ʈ�� �����, �̿� ���� �ڵ��� ��ȯ.

		��, ����, �� �Ǵ� ������ �����̵� �߰��Ϸ��� ��� ȿ�� ���ؽ�Ʈ�� ���� Ŭ������ �з��Ѵ�. 
		�ڵ��� ���� ȿ�� ���ؽ�Ʈ�� �����ͷ� �����ϴ� �淮 ����, ������(data)��� �Ѵ�.
		data�� �����÷��� ȿ�� ���ؽ�Ʈ ������ ���� �������̴�. data == ���� ���ؽ�Ʈ
		�ڵ� == �� ���� ��ƿ��Ƽ�� ���Ե� ����
		���ؽ�Ʈ == ���� �����÷��� ȿ�� ���ؽ�Ʈ�� �����ϴ� ���۸� �����ϴ� ���
	*/
	FGameplayEffectContextHandle ecHandle = targetASC->MakeEffectContext();

	// Source Object : ȿ���� ����
	ecHandle.AddSourceObject(this);

	/*
		1. ��� ASC�� Ư�� Gameplay Effect class�� �־����� Effect Spec�� ���� �� �����Ƿ� ����� ASC�� ����Ѵ�.
		2. Gameplay Effect Level : GameplayEffect�� ������ ������ ���� �� ������ ���� ������Ʈ�� �� Ŀ���� ���� 
		���� �ٸ� ������ ���� �ȴ�. ������ �׳� 1�� �����Ѵ�.
		Spec Handle : Spec�� �����ϴ� ����
		FGameplayEffectSpecHandle : �������Ʈ�� �����÷��� ȿ�� ������ �� �� �����ϰ� �ڵ�� ������ �� �ֵ��� ����Ѵ�.
									���� ��� � ���� �� �����Ѵ�.
		�ڵ鿡�� ���� ����ü�� �����ϴ� ���� ������ ���(data)�� �ִ�.
	*/
	const FGameplayEffectSpecHandle esHandle = targetASC->MakeOutgoingSpec(gmaePlayEffectClass, mActorLevel, ecHandle);

	const FActiveGameplayEffectHandle agpeHandle = targetASC->ApplyGameplayEffectSpecToSelf(*esHandle.Data.Get());

	const bool isInfinite = esHandle.Data.Get()->Def->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (isInfinite && mInfiniteEffectRemovePolicy == EEffectRemovePolicy::RemoveOnEndOverlap)
	{
		mActiveEffectHandles.Add(agpeHandle, targetASC);
	}
}

void AAGEffectActor::OnBeginOverlap(AActor* targetActor)
{
	if (mInstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(targetActor, mInstantGameplayEffectClass);
	}
	if (mDurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(targetActor, mDurationGameplayEffectClass);
	}
	if (mInfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(targetActor, mInfiniteGameplayEffectClass);
	}
}

void AAGEffectActor::OnEndOverlap(AActor* targetActor)
{
	/*
	* Overlap�� ���� �� Effect ����
	*/
	if (mInstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(targetActor, mInstantGameplayEffectClass);
	}
	if (mDurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(targetActor, mDurationGameplayEffectClass);
	}
	if (mInfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(targetActor, mInfiniteGameplayEffectClass);
	}

	/*
	* Overlap�� ���� �� Infinite Effect ����
	*/
	if (mInfiniteEffectRemovePolicy == EEffectRemovePolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(targetActor);
		if (!IsValid(targetASC)) return;

		TArray<FActiveGameplayEffectHandle> handlesToRemove;

		for (auto at : mActiveEffectHandles)
		{
			if (at.Value == targetASC)
			{
				targetASC->RemoveActiveGameplayEffect(at.Key, 1);
				handlesToRemove.Add(at.Key);
			}
		}

		for (auto& at : handlesToRemove)
		{
			mActiveEffectHandles.FindAndRemoveChecked(at);
		}
	}
}
