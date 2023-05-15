// Fill out your copyright notice in the Description page of Project Settings.


#include "Khaimera.h"
#include "../Particle/ParticleCascade.h"

AKhaimera::AKhaimera()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(100.f);
	GetCapsuleComponent()->SetCapsuleRadius(30.f);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -100.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	
	
	mMonsterTableRowName = TEXT("Khaimera");


	//-----------------------------------------
	// HitActor.
	//-----------------------------------------
	mHitActor = CreateDefaultSubobject<AParticleCascade>(TEXT("HitParticle"));

	AParticleCascade* particle = Cast<AParticleCascade>(mHitActor);
	particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonKhaimera/FX/ParticleSystems/Abilities/Ultimate/FX/P_Ult_Impact_DMG.P_Ult_Impact_DMG'"));
	//particle->SetActorScale3D(FVector(3.f, 3.f, 3.f));
}

void AKhaimera::BeginPlay()
{
	Super::BeginPlay();
}

void AKhaimera::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AKhaimera::UnPossessed()
{
	Super::UnPossessed();
}

void AKhaimera::NormalAttackCheck()
{
	Super::NormalAttackCheck();

	FVector startPosition = GetActorLocation() + GetActorForwardVector() * 10.f;
	FVector endPosition = startPosition + GetActorForwardVector() * mInfo.attackDistance;

	FCollisionQueryParams	param(NAME_None, false, this);

	TArray<FHitResult>	collisionResult;

	bool IsCollision = GetWorld()->SweepMultiByChannel(
		collisionResult, startPosition,
		endPosition, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel6,
		FCollisionShape::MakeSphere(mInfo.attackDistance),
		param);


#if ENABLE_DRAW_DEBUG

	//// CollisionEnable 가 true이면 Red, false이면 Green을 저장한다.
	//FColor	DrawColor = IsCollision ? FColor::Red : FColor::Green;

	//// FRotationMatrix::MakeFromZ(GetActorForwardVector()) : 앞쪽을
	//// 바라보는 회전행렬을 만들어서 .ToQuat() 함수를 이용하여 회전행렬을
	//// 회전값으로 변환해준다.
	//DrawDebugCapsule(GetWorld(), (startPosition + endPosition) / 2.f,
	//	mInfo.attackDistance / 2.f,
	//	mInfo.attackDistance,
	//	FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(),
	//	DrawColor, false, 0.5f);

#endif


	if (IsCollision)
	{
		int32	Count = collisionResult.Num();

		for (int32 i = 0; i < Count; ++i)
		{
			FActorSpawnParameters	SpawnParam;
			//SpawnParam.Template = mHitActor;
			SpawnParam.SpawnCollisionHandlingOverride =
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			// Hit 파티클 렌더링.
			AParticleCascade* Particle =
				GetWorld()->SpawnActor<AParticleCascade>(
					collisionResult[i].ImpactPoint,
					collisionResult[i].ImpactNormal.Rotation(),
					SpawnParam);

			Particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonKhaimera/FX/ParticleSystems/Abilities/Ultimate/FX/P_Ult_Impact_DMG.P_Ult_Impact_DMG'"));

			// 데미지 계산.
			collisionResult[i].GetActor()->TakeDamage(
				(float)mInfo.attackPoint,
				FDamageEvent(),
				GetController(),
				this);
		}
	}
}
