// Fill out your copyright notice in the Description page of Project Settings.


#include "AGAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
UAGAttributeSet::UAGAttributeSet()
{
	InitmHp(50.f);
	InitmMaxHp(100.f);
	InitmMp(100.f);
	InitmMaxMp(100.f);
}

void UAGAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 복제하려는 모든 항목에 대한 복제할 상태를 등록한다.
	// - COND_None : 아무런 조건없이 복제된다.
	DOREPLIFETIME_CONDITION_NOTIFY(UAGAttributeSet, mHp, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAGAttributeSet, mMaxHp, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAGAttributeSet, mMp, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAGAttributeSet, mMaxMp, COND_None, REPNOTIFY_Always);
}

void UAGAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// 인수가 Hp 속성인가?
	if (Attribute == GetmHpAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetmMaxHp());
	}
	// 인수가 Mp 속성인가?
	else if (Attribute == GetmMpAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetmMaxMp());
	}
}

void UAGAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	FEffectProperties props;
	SetEffectProperties(Data, props);
}

void UAGAttributeSet::OnRep_Hp(const FGameplayAttributeData& preHp) const
{
	// 변경 사항을 시스템에 알린다.
	// 우리가 값을 복제하고 있으며 해당 값이 서버에서 내려와 변경되었으며 
	// 이제 능력 시스템이 해당 변경 사항을 등록하고 필요한 경우를 대비하여 
	// 이전 값을 추적할 수 있다는 사실을 능력 시스템에 알리는 역할
	// 서버는 변경 사항이 유효하지 않다고 판단되면 변경 사항을 롤백하고 실행 취소할 수 있습니다.
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAGAttributeSet, mHp, preHp);
}

void UAGAttributeSet::OnRep_MaxHp(const FGameplayAttributeData& prMaxHp) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAGAttributeSet, mMaxHp, prMaxHp);
}

void UAGAttributeSet::OnRep_Mp(const FGameplayAttributeData& preMp) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAGAttributeSet, mMp, preMp);
}

void UAGAttributeSet::OnRep_MaxMp(const FGameplayAttributeData& prMaxeMp) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAGAttributeSet, mMaxMp, prMaxeMp);
}

void UAGAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	/**
	 * 변경된 Attribute 종류 : Data.EvaluatedData.Attribute
	 * 변경된 Attribute 크기 : Data.EvaluatedData.Magnitude
	 *
	 */

	 // source = causer of the effect
	 // target = target of the effect, owner of ths AS
	Props.effectContextHandle = Data.EffectSpec.GetContext();
	Props.sourceASC = Props.effectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	if (IsValid(Props.sourceASC) && Props.sourceASC->AbilityActorInfo.IsValid() && Props.sourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.sourceAvatarActor = Props.sourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.sourceController = Props.sourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.sourceController == nullptr && Props.sourceAvatarActor != nullptr)
		{
			if (const APawn* pawn = Cast<APawn>(Props.sourceAvatarActor))
			{
				Props.sourceController = pawn->GetController();
			}
		}
		if (Props.sourceController)
		{
			Props.sourceCharacter = Cast<ACharacter>(Props.sourceController->GetPawn());
		}
	}

	// Get Target
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.targetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.targetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.targetCharacter = Cast<ACharacter>(Props.targetAvatarActor);
		Props.targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.targetAvatarActor);
	}
}
