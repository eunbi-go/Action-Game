// Fill out your copyright notice in the Description page of Project Settings.


#include "FallingSwordAbility.h"
#include "../../Player/Valkyrie.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "../../Particle/Valkyrie/ValkyrieFallingSwordEffect.h"

void UFallingSwordAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UFallingSwordAbility::SpawnEffect()
{
	bool isServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!isServer) return;

	AActor* owner = GetAvatarActorFromActorInfo();

	if (AValkyrie* valkyrie = Cast<AValkyrie>(owner))
	{
		valkyrie->CameraSwitch(true);
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

	// ����Ʈ�� ���ظ� ������ ���� Gameplay Effect Spec ����
	const UAbilitySystemComponent* sourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	const FGameplayEffectSpecHandle specHandle = sourceASC->MakeOutgoingSpec(mDamageEffectClass, GetAbilityLevel(), sourceASC->MakeEffectContext());
	effect->mDamageEffectSpecHandle = specHandle;


	// ����Ʈ ����
	effect->FinishSpawning(spawnTransform);
}

void UFallingSwordAbility::AbilityEnd()
{
	AActor* owner = GetAvatarActorFromActorInfo();

	if (AValkyrie* valkyrie = Cast<AValkyrie>(owner))
	{
		valkyrie->CameraSwitch(false);
	}
}