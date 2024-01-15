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

	// �����Ϸ��� ��� �׸� ���� ������ ���¸� ����Ѵ�.
	// - COND_None : �ƹ��� ���Ǿ��� �����ȴ�.
	DOREPLIFETIME_CONDITION_NOTIFY(UAGAttributeSet, mHp, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAGAttributeSet, mMaxHp, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAGAttributeSet, mMp, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAGAttributeSet, mMaxMp, COND_None, REPNOTIFY_Always);
}

void UAGAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// �μ��� Hp �Ӽ��ΰ�?
	if (Attribute == GetmHpAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetmMaxHp());
	}
	// �μ��� Mp �Ӽ��ΰ�?
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
	// ���� ������ �ý��ۿ� �˸���.
	// �츮�� ���� �����ϰ� ������ �ش� ���� �������� ������ ����Ǿ����� 
	// ���� �ɷ� �ý����� �ش� ���� ������ ����ϰ� �ʿ��� ��츦 ����Ͽ� 
	// ���� ���� ������ �� �ִٴ� ����� �ɷ� �ý��ۿ� �˸��� ����
	// ������ ���� ������ ��ȿ���� �ʴٰ� �ǴܵǸ� ���� ������ �ѹ��ϰ� ���� ����� �� �ֽ��ϴ�.
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
	 * ����� Attribute ���� : Data.EvaluatedData.Attribute
	 * ����� Attribute ũ�� : Data.EvaluatedData.Magnitude
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
