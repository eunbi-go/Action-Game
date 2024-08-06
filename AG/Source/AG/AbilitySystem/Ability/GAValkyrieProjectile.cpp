// Fill out your copyright notice in the Description page of Project Settings.


#include "GAValkyrieProjectile.h"
#include "../../Particle/Valkyrie/ValkyrieProjectile.h"
#include "../../Player/Valkyrie.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UGAValkyrieProjectile::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData
)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	//PrintViewport(3.f, FColor::White, FString("UGAValkyrieProjectile::ActivateAbility"));
}

void UGAValkyrieProjectile::SpawnProjectile(const FVector& TargetLocation)
{
	mProjectileClass = AValkyrieProjectile::StaticClass();

	/*
	* ������ �ƴ� "����"���� �߻�ü�� �����Ѵ�.
	* �߻�ü�� replicated actor �� �Ǳ⸦ ���Ѵ�.
	* - ������ �߻�ü�� �����ϸ� ������ �߻�ü �̵�, ��ġ ó�� �� ��Ÿ ���� �۾��� ����ϰ� �Ǹ�
	*   Ŭ���̾�Ʈ�� �߻�ü�� ������ ������ ���� �ȴ�.
	*/


	bool isServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!isServer) return;

	// �������� �߻�ü�� �����Ѵ�.
	FTransform spawnTransform = FTransform();
	const FVector& location = GetAvatarActorFromActorInfo()->GetActorLocation();
	FRotator rotation = (TargetLocation - location).Rotation();
	rotation.Pitch = 0.f;

	spawnTransform.SetLocation(location);
	spawnTransform.SetRotation(rotation.Quaternion());

	AValkyrieProjectile* projectile = GetWorld()->SpawnActorDeferred<AValkyrieProjectile>(
		mProjectileClass,
		spawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);
	projectile->SetParticle(FString("NiagaraSystem'/Game/sA_PickupSet_1/Fx/NiagaraSystems/NS_Pickup_3.NS_Pickup_3'"));


	// �߻�ü�� ���ظ� ������ ���� Gameplay Effect Spec ����
	const UAbilitySystemComponent* sourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	const FGameplayEffectSpecHandle specHandle = sourceASC->MakeOutgoingSpec(mDamageEffectClass, GetAbilityLevel(), sourceASC->MakeEffectContext());
	projectile->mDamageEffectSpecHandle = specHandle;


	// �߻�ü ����
	projectile->FinishSpawning(spawnTransform);
}
