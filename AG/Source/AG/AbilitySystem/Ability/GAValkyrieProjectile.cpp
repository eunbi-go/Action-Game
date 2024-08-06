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
	* 로컬이 아닌 "서버"에서 발사체를 생성한다.
	* 발사체가 replicated actor 가 되기를 원한다.
	* - 서버가 발사체를 생성하면 서버는 발사체 이동, 위치 처리 및 기타 좋은 작업을 담당하게 되며
	*   클라이언트는 발사체의 복제된 버전을 보게 된다.
	*/


	bool isServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!isServer) return;

	// 서버에서 발사체를 생성한다.
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


	// 발사체에 피해를 입히기 위한 Gameplay Effect Spec 제공
	const UAbilitySystemComponent* sourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	const FGameplayEffectSpecHandle specHandle = sourceASC->MakeOutgoingSpec(mDamageEffectClass, GetAbilityLevel(), sourceASC->MakeEffectContext());
	projectile->mDamageEffectSpecHandle = specHandle;


	// 발사체 스폰
	projectile->FinishSpawning(spawnTransform);
}
