// Fill out your copyright notice in the Description page of Project Settings.


#include "SprintAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "../../Player/Valkyrie.h"
#include "../../Particle/Valkyrie/ValkyrieSprintEffect.h"
#include "AbilitySystemComponent.h"

void USprintAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AActor* owner = GetAvatarActorFromActorInfo();
	AAGPlayer* player = Cast<AAGPlayer>(owner);
	if (IsValid(player))
	{
		player->SetActionState(EActionState::EAS_Attack_Skill, true);
		player->SetSkillState(ESkillState::ESS_Sprint);
	}
}

void USprintAbility::SpawnEffect(const FVector& TargetLocation)
{
	//PrintViewport(10.f, FColor::Yellow, FString("USprintAbility::SpawnEffect"));

	bool isServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!isServer) return;


	AActor* owner = GetAvatarActorFromActorInfo();
	AAGPlayer* player = Cast<AAGPlayer>(owner);
	if (IsValid(player))
	{
		player->SetActionState(EActionState::EAS_Attack_Skill, false);
		player->SetSkillState(ESkillState::ESS_None);
	}


	// 서버에서 이펙트를 생성한다.
	FTransform spawnTransform = FTransform();
	const FVector& location = GetAvatarActorFromActorInfo()->GetActorLocation();
	spawnTransform.SetLocation(location);

	
	AValkyrieSprintEffect* effect = GetWorld()->SpawnActorDeferred<AValkyrieSprintEffect>(
		mEffectClass,
		spawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);
	//effect->SetParticle(FString("NiagaraSystem'/Game/NiagaraMagicalSlashes/Fx/Slashes/NS_SlashRing_01.NS_SlashRing_01'"));


	// 이펙트에 피해를 입히기 위한 Gameplay Effect Spec 제공
	const UAbilitySystemComponent* sourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	const FGameplayEffectSpecHandle specHandle = sourceASC->MakeOutgoingSpec(mDamageEffectClass, GetAbilityLevel(), sourceASC->MakeEffectContext());
	effect->mDamageEffectSpecHandle = specHandle;


	// 이펙트 스폰
	effect->FinishSpawning(spawnTransform);
}

void USprintAbility::SetTarget(const FVector& TargetLocation)
{
	bool isServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!isServer) return;

	AValkyrie* valkyrie = Cast<AValkyrie>(GetAvatarActorFromActorInfo());
	if (!IsValid(valkyrie)) return;
	valkyrie->SetMotionWarpingComponent(TargetLocation);
}
