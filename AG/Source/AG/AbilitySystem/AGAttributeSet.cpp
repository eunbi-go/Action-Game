// Fill out your copyright notice in the Description page of Project Settings.


#include "AGAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "../AGGameplayTags.h"

UAGAttributeSet::UAGAttributeSet()
{
	const FAGGameplayTags& gameplayTags = FAGGameplayTags::Get();

	FAttributeSignature strengthDelegate;
	strengthDelegate.BindStatic(UAGAttributeSet::GetmStrengthAttribute);
	mTagsToAttributes.Add(gameplayTags.Attributes_Primary_Strength, strengthDelegate);

	FAttributeSignature intelligenceDelegate;
	intelligenceDelegate.BindStatic(UAGAttributeSet::GetmIntelligenceAttribute);
	mTagsToAttributes.Add(gameplayTags.Attributes_Primary_Intelligence, intelligenceDelegate);

	FAttributeSignature dexDelegate;
	dexDelegate.BindStatic(UAGAttributeSet::GetmDexterityAttribute);
	mTagsToAttributes.Add(gameplayTags.Attributes_Primary_Dexterity, dexDelegate);

	FAttributeSignature resilienceDelegate;
	resilienceDelegate.BindStatic(UAGAttributeSet::GetmResilienceAttribute);
	mTagsToAttributes.Add(gameplayTags.Attributes_Primary_Resilience, resilienceDelegate);

	FAttributeSignature coinDelegate;
	coinDelegate.BindStatic(UAGAttributeSet::GetmCoinAttribute);
	mTagsToAttributes.Add(gameplayTags.Attributes_Primary_Coin, coinDelegate);







	FAttributeSignature armorDelegate;
	armorDelegate.BindStatic(UAGAttributeSet::GetmArmorAttribute);
	mTagsToAttributes.Add(gameplayTags.Attributes_Second_Armor, armorDelegate);

	FAttributeSignature blockPercentDelegate;
	blockPercentDelegate.BindStatic(UAGAttributeSet::GetmBlockPercentAttribute);
	mTagsToAttributes.Add(gameplayTags.Attributes_Second_BlockPercent, blockPercentDelegate);

	FAttributeSignature criticalPercentDelegate;
	criticalPercentDelegate.BindStatic(UAGAttributeSet::GetmCriticalPercentAttribute);
	mTagsToAttributes.Add(gameplayTags.Attributes_Second_CriticalPercent, criticalPercentDelegate);

	FAttributeSignature criticalDamageDelegate;
	criticalDamageDelegate.BindStatic(UAGAttributeSet::GetmCriticalDamageAttribute);
	mTagsToAttributes.Add(gameplayTags.Attributes_Second_CriticalDamage, criticalDamageDelegate);

	FAttributeSignature criticalResilienceDelegate;
	criticalResilienceDelegate.BindStatic(UAGAttributeSet::GetmCriticalResistanceAttribute);
	mTagsToAttributes.Add(gameplayTags.Attributes_Second_CriticalResistance, criticalResilienceDelegate);

	FAttributeSignature maxHpDelegate;
	maxHpDelegate.BindStatic(UAGAttributeSet::GetmMaxHpAttribute);
	mTagsToAttributes.Add(gameplayTags.Attributes_Second_MaxHp, maxHpDelegate);

	FAttributeSignature maxMpDelegate;
	maxMpDelegate.BindStatic(UAGAttributeSet::GetmMaxMpAttribute);
	mTagsToAttributes.Add(gameplayTags.Attributes_Second_MaxMp, maxMpDelegate);

	//FAttributeSignature intelligenceDelegate;
	//intelligenceDelegate.BindStatic(UAGAttributeSet::GetmIntelligenceAttribute);
	//mTagsToAttributes.Add(gameplayTags.Attributes_Primary_Intelligence, intelligenceDelegate);

	//mFunctionPointer = GetmIntelligenceAttribute;
	//FGameplayAttribute attribute = mFunctionPointer();
}

void UAGAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// �����Ϸ��� ��� �׸� ���� ������ ���¸� ����Ѵ�.
	// - COND_None : �ƹ��� ���Ǿ��� �����ȴ�.



	// first
	DOREPLIFETIME_CONDITION_NOTIFY(UAGAttributeSet, mStrength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAGAttributeSet, mDexterity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAGAttributeSet, mIntelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAGAttributeSet, mResilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAGAttributeSet, mCoin, COND_None, REPNOTIFY_Always);

	// second
	DOREPLIFETIME_CONDITION_NOTIFY(UAGAttributeSet, mArmor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAGAttributeSet, mBlockPercent, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAGAttributeSet, mCriticalPercent, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAGAttributeSet, mCriticalResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAGAttributeSet, mCriticalDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAGAttributeSet, mMaxHp, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAGAttributeSet, mMaxMp, COND_None, REPNOTIFY_Always);

	// vital
	DOREPLIFETIME_CONDITION_NOTIFY(UAGAttributeSet, mHp, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAGAttributeSet, mMp, COND_None, REPNOTIFY_Always);

}

void UAGAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// �����ڸ� �����Ͽ� ��ȯ�Ǵ� ���� ����ȴ�. 
	// ���� hp�� �������� �ʴ´�. 

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

	// �Ӽ� Ȯ��
	if (Data.EvaluatedData.Attribute == GetmHpAttribute())
	{
		// ���� hp�� �����Ѵ�. 
		SetmHp(FMath::Clamp(GetmHp(), 0.f, GetmMaxHp()));
		//FString::Printf(TEXT("index : %d"), mSlashSkillIndex));
		PrintViewport(5.f, FColor::White, FString::Printf(TEXT("%s hp %f consume"), *props.targetAvatarActor->GetName(), GetmHp()));
	}
	else if (Data.EvaluatedData.Attribute == GetmMpAttribute())
	{
		SetmMp(FMath::Clamp(GetmMp(), 0.f, GetmMaxMp()));
	}
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

void UAGAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& preIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAGAttributeSet, mIntelligence, preIntelligence);
}

void UAGAttributeSet::OnRep_Strength(const FGameplayAttributeData& PreStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAGAttributeSet, mStrength, PreStrength);
}

void UAGAttributeSet::OnRep_Resilience(const FGameplayAttributeData& PreResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAGAttributeSet, mResilience, PreResilience);
}

void UAGAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& PreDexterity) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAGAttributeSet, mDexterity, PreDexterity);
}

void UAGAttributeSet::OnRep_Coin(const FGameplayAttributeData& preCoin) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAGAttributeSet, mCoin, preCoin);
}

void UAGAttributeSet::OnRep_Armor(const FGameplayAttributeData& PreArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAGAttributeSet, mArmor, PreArmor);
}

void UAGAttributeSet::OnRep_BlockPercent(const FGameplayAttributeData& PreBlockPercent) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAGAttributeSet, mBlockPercent, PreBlockPercent);
}

void UAGAttributeSet::OnRep_CriticalPercent(const FGameplayAttributeData& PreCriticalPercent) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAGAttributeSet, mCriticalPercent, PreCriticalPercent);
}

void UAGAttributeSet::OnRep_CriticalDamage(const FGameplayAttributeData& PreCriticalDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAGAttributeSet, mCriticalDamage, PreCriticalDamage);
}

void UAGAttributeSet::OnRep_CriticalResistance(const FGameplayAttributeData& PreCriticalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAGAttributeSet, mCriticalResistance, PreCriticalResistance);
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

