// Fill out your copyright notice in the Description page of Project Settings.


#include "MMC_MaxHp.h"
#include "../AGAttributeSet.h"
#include "../../Interface/CombatInterface.h"

UMMC_MaxHp::UMMC_MaxHp()
{
	//----------------------
	// Modifier가 실행되고, Effect가 적용되면,
	// 적용 대상에서 Resilience 값이 캡처되고
	// CalculateBaseMagnitude_Implementation() 에서 사용할 수 있다.
	//----------------------

	// 생성자에서 Resilience 속성을 캡처한다.
	mResilienceDef.AttributeToCapture = UAGAttributeSet::GetmResilienceAttribute();

	// 타겟의 Resilence 속성을 캡처한다.
	// Valkyrie에서 Valkyrie로 GameplayEffect를 적용하고 있기 때문에
	// 여기서는 Source와 Target이 동일하다.
	mResilienceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

	// Snapshot 여부 결정
	// Snapshot은 타이밍과 관련됨
	// GameplayEffect를 생성과 동시에 Effect를 적용하고 싶지 않을 때 사용
	mResilienceDef.bSnapshot = false;


	// 캡처할 속성의 배열
	RelevantAttributesToCapture.Add(mResilienceDef);
}

float UMMC_MaxHp::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//----------------------
	// Source 또는 Target이 갖고 있는 GameplayTag를 얻는다.
	//----------------------
	const FGameplayTagContainer* sourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* targetTags = Spec.CapturedTargetTags.GetAggregatedTags();


	//----------------------
	// Attribute를 캡처하고, 그 Attribute를 사용하기 위해
	// FAggregatorEvaluateParameters 구조체 생성
	//----------------------
	FAggregatorEvaluateParameters evaluationParams;
	evaluationParams.SourceTags = sourceTags;
	evaluationParams.TargetTags = targetTags;


	//----------------------
	// Attribute : Resilience 를 캡처한다.
	// 실제 Resilience 값을 얻는다.
	// 음수의 값을 갖지 않게 Clmap 한다.
	//----------------------
	float resilience = 0.f;
	GetCapturedAttributeMagnitude(mResilienceDef, Spec, evaluationParams, resilience);
	resilience = FMath::Max<float>(resilience, 0.f);

	//----------------------
	// Attribute가 적용되는 객체의 Combat Interface를 얻어서,
	// 객체의 Level을 확인한다.
	//----------------------
	ICombatInterface* combatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 playerLevel = combatInterface->GetPlayerLevel();


	//----------------------
	// MaxHp를 Resilience와 Level을 이용해 원하는 값으로 설정한다.
	//----------------------
	return 80.f + (2.5f * resilience) + (10.f * playerLevel);
}
