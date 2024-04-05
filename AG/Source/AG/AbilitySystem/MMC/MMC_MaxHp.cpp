// Fill out your copyright notice in the Description page of Project Settings.


#include "MMC_MaxHp.h"
#include "../AGAttributeSet.h"
#include "../../Interface/CombatInterface.h"

UMMC_MaxHp::UMMC_MaxHp()
{
	//----------------------
	// Modifier�� ����ǰ�, Effect�� ����Ǹ�,
	// ���� ��󿡼� Resilience ���� ĸó�ǰ�
	// CalculateBaseMagnitude_Implementation() ���� ����� �� �ִ�.
	//----------------------

	// �����ڿ��� Resilience �Ӽ��� ĸó�Ѵ�.
	mResilienceDef.AttributeToCapture = UAGAttributeSet::GetmResilienceAttribute();

	// Ÿ���� Resilence �Ӽ��� ĸó�Ѵ�.
	// Valkyrie���� Valkyrie�� GameplayEffect�� �����ϰ� �ֱ� ������
	// ���⼭�� Source�� Target�� �����ϴ�.
	mResilienceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

	// Snapshot ���� ����
	// Snapshot�� Ÿ�ְ̹� ���õ�
	// GameplayEffect�� ������ ���ÿ� Effect�� �����ϰ� ���� ���� �� ���
	mResilienceDef.bSnapshot = false;


	// ĸó�� �Ӽ��� �迭
	RelevantAttributesToCapture.Add(mResilienceDef);
}

float UMMC_MaxHp::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//----------------------
	// Source �Ǵ� Target�� ���� �ִ� GameplayTag�� ��´�.
	//----------------------
	const FGameplayTagContainer* sourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* targetTags = Spec.CapturedTargetTags.GetAggregatedTags();


	//----------------------
	// Attribute�� ĸó�ϰ�, �� Attribute�� ����ϱ� ����
	// FAggregatorEvaluateParameters ����ü ����
	//----------------------
	FAggregatorEvaluateParameters evaluationParams;
	evaluationParams.SourceTags = sourceTags;
	evaluationParams.TargetTags = targetTags;


	//----------------------
	// Attribute : Resilience �� ĸó�Ѵ�.
	// ���� Resilience ���� ��´�.
	// ������ ���� ���� �ʰ� Clmap �Ѵ�.
	//----------------------
	float resilience = 0.f;
	GetCapturedAttributeMagnitude(mResilienceDef, Spec, evaluationParams, resilience);
	resilience = FMath::Max<float>(resilience, 0.f);

	//----------------------
	// Attribute�� ����Ǵ� ��ü�� Combat Interface�� ��,
	// ��ü�� Level�� Ȯ���Ѵ�.
	//----------------------
	ICombatInterface* combatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 playerLevel = combatInterface->GetPlayerLevel();


	//----------------------
	// MaxHp�� Resilience�� Level�� �̿��� ���ϴ� ������ �����Ѵ�.
	//----------------------
	return 80.f + (2.5f * resilience) + (10.f * playerLevel);
}
