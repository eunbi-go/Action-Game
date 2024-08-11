// Fill out your copyright notice in the Description page of Project Settings.


#include "GAValkyrieProjectile.h"
#include "../../Particle/Valkyrie/ValkyrieProjectile.h"
#include "../../Player/Valkyrie.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "../../Player/AGPlayerController.h"
#include "../../Player/ValkyrieAnimInstance.h"

void UGAValkyrieProjectile::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData
)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	APlayerController* pc = GetCurrentActorInfo()->PlayerController.Get();
	AAGPlayerController* apc = Cast<AAGPlayerController>(pc);
	if (IsValid(apc))
	{
		apc->mOnRangeRelease.AddDynamic(this, &UGAValkyrieProjectile::SpawnProjectile);

		apc->mPressingTime = 0.f;
		apc->mIsCheckPressingTime = false;
	}

	AActor* actor = GetAvatarActorFromActorInfo();
	const FVector& location = actor->GetActorLocation();
	const FVector& forward = actor->GetActorForwardVector();
	const FVector& right = actor->GetActorRightVector();

	FVector loc1 = location + forward * mOffset;
	mLocationArray.Add(loc1);
	mDirectionArray.Add(forward);
	
	loc1 = location - forward * mOffset;
	mLocationArray.Add(loc1);
	FVector dir1 = forward;
	dir1.X *= -1;
	mDirectionArray.Add(dir1);

	FVector loc2 = location + right * mOffset;
	mLocationArray.Add(loc2);
	mDirectionArray.Add(right);
	
	loc2 = location - right * mOffset;
	mLocationArray.Add(loc2);
	FVector dir2 = right;
	dir2.Y *= -1;
	mDirectionArray.Add(dir2);

	FVector loc3 = location;
	loc3 += forward * mOffset;
	loc3 += right * mOffset;
	mLocationArray.Add(loc3);
	FVector dir3 = forward + right;
	mDirectionArray.Add(dir3);

	FVector loc4 = location;
	loc4 += forward * mOffset;
	loc4 -= right * mOffset;
	mLocationArray.Add(loc4);
	FVector dir4 = forward;
	dir4 -= right;
	mDirectionArray.Add(dir4);

	FVector loc5 = location;
	loc5 -= forward * mOffset;
	loc5 -= right * mOffset;
	mLocationArray.Add(loc5);
	FVector dir5 = -forward;
	dir5 -= right;
	mDirectionArray.Add(dir5);

	FVector loc6 = location;
	loc6 -= forward * mOffset;
	loc6 += right * mOffset;
	mLocationArray.Add(loc6);
	FVector dir6 = -forward;
	dir6 += right;
	mDirectionArray.Add(dir6);
}

void UGAValkyrieProjectile::SpawnProjectile(const float& Time)
{
	/*
	* 로컬이 아닌 "서버"에서 발사체를 생성한다.
	* 발사체가 replicated actor 가 되기를 원한다.
	* - 서버가 발사체를 생성하면 서버는 발사체 이동, 위치 처리 및 기타 좋은 작업을 담당하게 되며
	*   클라이언트는 발사체의 복제된 버전을 보게 된다.
	*/

	bool isServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!isServer) return;
	if (mIsSpawnEffect)
		return;

	GetWorld()->GetTimerManager().ClearTimer(mTimer);
	mIsSpawnEffect = true;
	UAnimMontage* montage = GetCurrentMontage();
	AValkyrie* valkyrie = Cast<AValkyrie>(GetAvatarActorFromActorInfo());
	if (IsValid(valkyrie) && !valkyrie->GetAnimInst()->Montage_IsPlaying(montage))
	{
		valkyrie->GetAnimInst()->Montage_Resume(montage);
	}

	mOnProjectileStart.Broadcast();
}

void UGAValkyrieProjectile::Pause()
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


	GetWorld()->GetTimerManager().SetTimer(mTimer, this, &UGAValkyrieProjectile::SpawnEffect, 1.0f, true, 2.0f);
}

void UGAValkyrieProjectile::SpawnEffect()
{
	bool isServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!isServer) return;

	if (mSpawnCount == 8)
	{
		SpawnProjectile(0.f);
		return;
	}
	PrintViewport(5.f, FColor::Green, FString("UGAValkyrieProjectile::SpawnEffect()"));

	// 서버에서 발사체를 생성한다.


	FTransform spawnTransform = FTransform();
	spawnTransform.SetLocation(mLocationArray[mSpawnCount]);

	AValkyrieProjectile* projectile = GetWorld()->SpawnActorDeferred<AValkyrieProjectile>(
		mProjectileClass,
		spawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);
	projectile->mProjectileMovement->Velocity = mDirectionArray[mSpawnCount];
	
	// 발사체에 피해를 입히기 위한 Gameplay Effect Spec 제공
	const UAbilitySystemComponent* sourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	const FGameplayEffectSpecHandle specHandle = sourceASC->MakeOutgoingSpec(mDamageEffectClass, GetAbilityLevel(), sourceASC->MakeEffectContext());
	projectile->mDamageEffectSpecHandle = specHandle;


	// 발사체 스폰
	projectile->FinishSpawning(spawnTransform);
	mOnProjectileStart.AddUObject(projectile, &AValkyrieProjectile::ProjectileStart);
	mSpawnCount++;
}
