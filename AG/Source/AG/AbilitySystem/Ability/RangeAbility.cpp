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


	// 이펙트에 피해를 입히기 위한 Gameplay Effect Spec 제공
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
	// 바로 해제함 -> 델리게이트 변수의 브로드캐스트로 이미 이펙트가 스폰됨
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