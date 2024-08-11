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

	// 복제하려는 모든 항목에 대한 복제할 상태를 등록한다.
	// - COND_None : 아무런 조건없이 복제된다.



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

	// 수식자를 쿼리하여 반환되는 값만 변경된다. 
	// 실제 hp를 변경하지 않는다. 

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

	// 속성 확인
	if (Data.EvaluatedData.Attribute == GetmHpAttribute())
	{
		// 실제 hp를 변경한다. 
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

