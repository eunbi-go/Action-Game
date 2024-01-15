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

// 하위 클래스가 GameplayEffect로 설정되어 있으면 target에 GameplayEffect를 적용한다.
void AAGEffectActor::ApplyEffectToTarget(AActor* target, TSubclassOf<UGameplayEffect> gmaePlayEffectClass)
{
	/*
		IAbilitySystemInterface* targetASC = Cast<IAbilitySystemInterface>(target)
		위의 방법 대신 아래와 같이 정적 함수 라이브러리 사용.
		GetAbilitySystemComponent() 함수 안에 위의 코드가 들어있음.
		모든 사람이 인터페이스를 구현하는 것이 아님. 
		GAS는 이러한 극단적인 경우를 처리하도록 설계된 전역 라이브러리가 있고, 
		그것이 UAbilitySystemBlueprintLibrary
	*/
	UAbilitySystemComponent* targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(target);
	if (targetASC == nullptr)
		return;
	check(gmaePlayEffectClass);

	/*
		게임플레이 효과는 효과 컨텍스트와 밀접한 관련이 있다. 
		게임플레이 효과와 이를 둘러싼 컨텍스트를 설명하는 방법에는 끝이 없다.(효과를 일으키는 원인과 대상, 어떤 효과인지 등등)
		게임플레이 효과 컨텍스트는 게임 플레이 효과, 특히 효과 컨텍스트를 실제로 전달하여 효과 사양과 연관시킨다.
		실제로 효과 컨텍스트는 핸들을 처리한다. 즉, 핸들은 컨텍스트 자체에 대한 포인터를 포함하는 것이다.

		Q. 컨텍스트 핸들을 어떻게 만드는가
		GAS에서 얻을 수 있다. 
		MakeEffectContext() : 효과 컨텍스트를 만들고, 이에 대한 핸들을 반환.

		불, 번개, 독 또는 실제로 무엇이든 추가하려는 경우 효과 컨텍스트를 하위 클래스로 분류한다. 
		핸들은 실제 효과 컨텍스트를 포인터로 저장하는 경량 래퍼, 데이터(data)라고 한다.
		data는 게임플레이 효과 컨텍스트 유형의 공유 포인터이다. data == 실제 컨텍스트
		핸들 == 몇 가지 유틸리티가 포함된 래퍼
		컨텍스트 == 내부 게임플레이 효과 컨텍스트를 포함하는 래퍼를 생성하는 방법
	*/
	FGameplayEffectContextHandle ecHandle = targetASC->MakeEffectContext();

	// Source Object : 효과의 원인
	ecHandle.AddSourceObject(this);

	/*
		1. 모든 ASC는 특정 Gameplay Effect class가 주어지면 Effect Spec을 만들 수 있으므로 대상의 ASC만 사용한다.
		2. Gameplay Effect Level : GameplayEffect는 고유한 레벨을 가질 수 있으며 게임 프로젝트가 더 커짐에 따라 
		서로 다른 수준을 갖게 된다. 지금은 그냥 1로 설정한다.
		Spec Handle : Spec을 보유하는 래퍼
		FGameplayEffectSpecHandle : 블루프린트가 게임플레이 효과 스펙을 한 번 생성하고 핸들로 참조할 수 있도록 허용한다.
									여러 대상 등에 여러 번 적용한다.
		핸들에는 실제 구조체를 저장하는 내부 데이터 멤버(data)가 있다.
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
	* Overlap이 끝날 때 Effect 적용
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
	* Overlap이 끝날 때 Infinite Effect 제거
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
