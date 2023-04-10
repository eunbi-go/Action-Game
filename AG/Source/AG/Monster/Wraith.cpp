// Fill out your copyright notice in the Description page of Project Settings.


#include "Wraith.h"
#include "../Particle/ParticleCascade.h"

AWraith::AWraith()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(88.f);
	GetCapsuleComponent()->SetCapsuleRadius(34.f);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));



	mMonsterTableRowName = TEXT("Wraith");


	//-----------------------------------------
	// HitActor.
	//-----------------------------------------
	mHitActor = CreateDefaultSubobject<AParticleCascade>(TEXT("HitParticle"));

	AParticleCascade* particle = Cast<AParticleCascade>(mHitActor);
	particle->SetParticle(TEXT("ParticleSystem'/Game/ParagonWraith/FX/Particles/Abilities/ScopedShot/FX/P_Wraith_Sniper_HitCharacter.P_Wraith_Sniper_HitCharacter'"));
}

void AWraith::BeginPlay()
{
	Super::BeginPlay();
}

void AWraith::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AWraith::UnPossessed()
{
	Super::UnPossessed();
}

void AWraith::NormalAttackCheck()
{
	Super::NormalAttackCheck();

	FVector gunPosition = GetMesh()->GetBoneLocation(TEXT("gun_barrel"));

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AParticleCascade* fireParticle = GetWorld()->SpawnActor<AParticleCascade>(
		gunPosition,
		GetActorForwardVector().Rotation(),
		spawnParams);

	fireParticle->SetParticle(TEXT("ParticleSystem'/Game/ParagonWraith/FX/Particles/Abilities/ScopedShot/FX/P_Wraith_Sniper_ExitWorld.P_Wraith_Sniper_ExitWorld'"));


	FVector endPosition = gunPosition + GetActorForwardVector() * mInfo.attackDistance;

	FCollisionQueryParams params(NAME_None, false, this);
	
	FHitResult result;
	bool IsCollision = GetWorld()->LineTraceSingleByChannel(result,
		gunPosition,
		endPosition,
		ECollisionChannel::ECC_GameTraceChannel6,
		params);



#if ENABLE_DRAW_DEBUG

	// CollisionEnable �� true�̸� Red, false�̸� Green�� �����Ѵ�.
	FColor	DrawColor = IsCollision ? FColor::Red : FColor::Green;

	// FRotationMatrix::MakeFromZ(GetActorForwardVector()) : ������
	// �ٶ󺸴� ȸ������� ���� .ToQuat() �Լ��� �̿��Ͽ� ȸ�������
	// ȸ�������� ��ȯ���ش�.
	DrawDebugLine(GetWorld(),
					gunPosition,
					endPosition,
					DrawColor,
					false,
					1.f, 2.f);

#endif


	if (IsCollision)
	{

		FActorSpawnParameters	SpawnParam;
		SpawnParam.Template = mHitActor;
		SpawnParam.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Hit ��ƼŬ ������.
		AParticleCascade* Particle =
			GetWorld()->SpawnActor<AParticleCascade>(
				result.ImpactPoint,
				result.ImpactNormal.Rotation(),
				SpawnParam);

		// ������ ���.
		result.GetActor()->TakeDamage(
			(float)mInfo.attackPoint,
			FDamageEvent(),
			GetController(),
			this);

	}
}