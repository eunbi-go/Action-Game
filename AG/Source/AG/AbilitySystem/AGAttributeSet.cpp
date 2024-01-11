// Fill out your copyright notice in the Description page of Project Settings.


#include "AGAttributeSet.h"
#include "Net/UnrealNetwork.h"

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
