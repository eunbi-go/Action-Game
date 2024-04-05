// Fill out your copyright notice in the Description page of Project Settings.


#include "MMC_MaxMp.h"
#include "../AGAttributeSet.h"
#include "../../Interface/CombatInterface.h"

UMMC_MaxMp::UMMC_MaxMp()
{
	//----------------------
	// Modifier�� ����ǰ�, Effect�� ����Ǹ�,
	// ���� ��󿡼� Resilience ���� ĸó�ǰ�
	// CalculateBaseMagnitude_Implementation() ���� ����� �� �ִ�.
	//----------------------

	// �����ڿ��� Resilience �Ӽ��� ĸó�Ѵ�.
	mIntelligenceDef.AttributeToCapture = UAGAttributeSet::GetmIntelligenceAttribute();

	// Ÿ���� Resilence �Ӽ��� ĸó�Ѵ�.
	// Valkyrie���� Valkyrie�� GameplayEffect�� �����ϰ� �ֱ� ������
	// ���⼭�� Source�� Target�� �����ϴ�.
	mIntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

	// Snapshot ���� ����
	// Snapshot�� Ÿ�ְ̹� ���õ�
	// GameplayEffect�� ������ ���ÿ� Effect�� �����ϰ� ���� ���� �� ���
	mIntelligenceDef.bSnapshot = false;


	// ĸó�� �Ӽ��� �迭
	RelevantAttributesToCapture.Add(mIntelligenceDef);
}

float UMMC_MaxMp::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
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
	// Attribute : Intelligence �� ĸó�Ѵ�.
	// ���� Intelligence ���� ��´�.
	// ������ ���� ���� �ʰ� Clmap �Ѵ�.
	//----------------------
	float intelligence = 0.f;
	GetCapturedAttributeMagnitude(mIntelligenceDef, Spec, evaluationParams, intelligence);
	intelligence = FMath::Max<float>(intelligence, 0.f);

	//----------------------
	// Attribute�� ����Ǵ� ��ü�� Combat Interface�� ��,
	// ��ü�� Level�� Ȯ���Ѵ�.
	//----------------------
	ICombatInterface* combatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 playerLevel = combatInterface->GetPlayerLevel();


	//----------------------
	// MaxHp�� Resilience�� Level�� �̿��� ���ϴ� ������ �����Ѵ�.
	//----------------------
	return 80.f + (2.5f * intelligence) + (10.f * playerLevel);
}
