// Fill out your copyright notice in the Description page of Project Settings.


#include "FallingSwordAbility.h"
#include "../../Player/Valkyrie.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "../../Particle/Valkyrie/ValkyrieFallingSwordEffect.h"

void UFallingSwordAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* owner = GetAvatarActorFromActorInfo();
	AValkyrie* player = Cast<AValkyrie>(owner);
	if (IsValid(player))
	{
		player->SetMp(-10);
		player->SetActionState(EActionState::EAS_Attack_Skill, true);
		player->SetSkillState(ESkillState::ESS_FallingSword);
	}
}

void UFallingSwordAbility::SpawnEffect()
{
	bool isServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!isServer) return;

	AActor* owner = GetAvatarActorFromActorInfo();

	if (AValkyrie* valkyrie = Cast<AValkyrie>(owner))
	{
		valkyrie->CameraSwitch(true);
		valkyrie->StartCameraShake(FName("Loc_Z"));
	}

	FTransform spawnTransform = FTransform();
	FVector location = owner->GetActorLocation();
	FVector forward = owner->GetActorForwardVector();
	location += forward * 100.f;
	location.Z = 0.f;
	spawnTransform.SetLocation(location);

	AValkyrieFallingSwordEffect* effect = GetWorld()->SpawnActorDeferred<AValkyrieFallingSwordEffect>(
		mEffectClass,
		spawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	// 이펙트에 피해를 입히기 위한 Gameplay Effect Spec 제공
	const UAbilitySystemComponent* sourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	const FGameplayEffectSpecHandle specHandle = sourceASC->MakeOutgoingSpec(mDamageEffectClass, GetAbilityLevel(), sourceASC->MakeEffectContext());
	effect->mDamageEffectSpecHandle = specHandle;


	// 이펙트 스폰
	effect->FinishSpawning(spawnTransform);
}

void UFallingSwordAbility::StartCameraShake()
{
	bool isServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!isServer) return;

	AActor* owner = GetAvatarActorFromActorInfo();

	if (AValkyrie* valkyrie = Cast<AValkyrie>(owner))
	{
		valkyrie->StartCameraShake(FName("Loc_Z"));
	}
}

void UFallingSwordAbility::AbilityEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(mTimer);

	AActor* owner = GetAvatarActorFromActorInfo();

	if (AValkyrie* valkyrie = Cast<AValkyrie>(owner))
	{
		valkyrie->CameraSwitch(false);
		valkyrie->SetActionState(EActionState::EAS_Attack_Skill, false);
		valkyrie->SetSkillState(ESkillState::ESS_None);
	}
}
