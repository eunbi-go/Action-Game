// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "../../Player/Valkyrie.h"
#include "AbilitySystemComponent.h"
#include "../../Player/ValkyrieAnimInstance.h"
#include "../../Particle/Valkyrie/RangeEffect.h"
#include "../../Player/AGPlayerController.h"

void URangeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* owner = GetAvatarActorFromActorInfo();
	AValkyrie* player = Cast<AValkyrie>(owner);
	if (IsValid(player))
	{
		player->SetMp(-10);
		player->SetActionState(EActionState::EAS_Attack_Skill, true);
		player->SetSkillState(ESkillState::ESS_Range);
	}

	APlayerController* pc = GetCurrentActorInfo()->PlayerController.Get();
	AAGPlayerController* apc = Cast<AAGPlayerController>(pc);
	if (IsValid(apc))
	{
		apc->mOnRangeRelease.AddDynamic(this, &URangeAbility::SpawnEffect);

		apc->mPressingTime = 0.f;
		apc->mIsCheckPressingTime = false;
	}
}

void URangeAbility::SpawnEffect(const float& PressingTime)
{
	bool isServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!isServer) return;

	PrintViewport(10.f, FColor::Orange, FString("Spawn2Effect"));
	UAnimMontage* montage = GetCurrentMontage();
	mIsSpawnEffect = true;

	AValkyrie* valkyrie = Cast<AValkyrie>(GetAvatarActorFromActorInfo());
	if (IsValid(valkyrie) && !valkyrie->GetAnimInst()->Montage_IsPlaying(montage))
	{
		valkyrie->GetAnimInst()->Montage_Resume(montage);
	}

	const FVector& location = GetAvatarActorFromActorInfo()->GetActorLocation();
	FTransform spawnTransform = FTransform();
	spawnTransform.SetLocation(location);

	ARangeEffect* effect = GetWorld()->SpawnActorDeferred<ARangeEffect>(
		mEffectClass,
		spawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	float size = 0.6 + PressingTime;
	if (PressingTime == 0.f)
		size = 0.3f;
	effect->SetActorScale3D(FVector(size));


	// ����Ʈ�� ���ظ� ������ ���� Gameplay Effect Spec ����
	const UAbilitySystemComponent* sourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	const FGameplayEffectSpecHandle specHandle = sourceASC->MakeOutgoingSpec(mDamageEffectClass, GetAbilityLevel(), sourceASC->MakeEffectContext());
	effect->mDamageEffectSpecHandle = specHandle;

	effect->FinishSpawning(spawnTransform);
}

void URangeAbility::Pause()
{
	bool isServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!isServer) return;
	PrintViewport(10.f, FColor::Orange, FString("Pau3se"));
	// Ű�� �ٷ� ������ -> ��������Ʈ ������ ��ε�ĳ��Ʈ�� �̹� ����Ʈ�� ������, �ִϸ��̼� ���� �ʿ�X
	if (mIsSpawnEffect)
	{
		//SpawnEffect(0.f);
		return;
	}

	APlayerController* pc = GetCurrentActorInfo()->PlayerController.Get();
	AAGPlayerController* apc = Cast<AAGPlayerController>(pc);
	if (IsValid(apc))
	{
		apc->mPressingTime = 0.f;
		apc->mIsCheckPressingTime = true;
	}


	UAnimMontage* montage = GetCurrentMontage();
	AValkyrie* valkyrie = Cast<AValkyrie>(GetAvatarActorFromActorInfo());
	if (IsValid(valkyrie))
	{
		valkyrie->GetAnimInst()->Montage_Pause(montage);
	}
}

void URangeAbility::AbilityEnd()
{
	AActor* owner = GetAvatarActorFromActorInfo();
	AAGPlayer* player = Cast<AAGPlayer>(owner);
	if (IsValid(player))
	{
		//PrintViewport(5.f, FColor::Orange, FString("URangeAbility::AbilityEnd()"));
		player->SetActionState(EActionState::EAS_Attack_Skill, false);
		player->SetSkillState(ESkillState::ESS_None);
	}
}
