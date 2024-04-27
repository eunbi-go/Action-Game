// Fill out your copyright notice in the Description page of Project Settings.


#include "ValkyrieSlash.h"
#include "../Interface/HitInterface.h"
#include "../Collision/CollisionActor.h"
#include "../Particle/ParticleCascade.h"
#include "../Particle/ParticleNiagara.h"
#include "../Player/Valkyrie.h"

AValkyrieSlash::AValkyrieSlash()
{
	mCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
}

void AValkyrieSlash::BeginPlay()
{
	Super::BeginPlay();
	
	FActorSpawnParameters	SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	mParticle->OnSystemFinished.AddDynamic(this, &AValkyrieSlash::OnSystemFinish);

	mCollisionActor = GetWorld()->SpawnActor<ACollisionActor>(
		GetActorLocation(),
		FRotator::ZeroRotator,
		SpawnParam
	);
	
	mCollisionActor->SetCollisionProfileName(FName("PlayerSword"));
	mCollisionActor->SetParent(this);
	mCollisionActor->SetOwnerActor(this);
	
}

void AValkyrieSlash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!mIsInit)
	{
		mIsInit = true;
		Initialize();
	}


	FVector location = mCollisionActor->GetRelativeLocation();
	if (mSlashIdx < 3)
		location += mDirection * 3000.f * DeltaTime;
	else
		location += mDirection * 500.f * DeltaTime;
	location.Z = GetActorLocation().Z;

	mCollisionActor->SetRelativeLoctaion(location);
}

void AValkyrieSlash::SpawnHitEffect(const FVector& Location, const FRotator& Rotator)
{
	FActorSpawnParameters	SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AParticleNiagara* Particle =
		GetWorld()->SpawnActor<AParticleNiagara>(
			Location,
			Rotator,
			SpawnParam);

	Particle->SetParticle(TEXT("NiagaraSystem'/Game/sA_StylizedAttacksPack/FX/NiagaraSystems/NS_BasicHit.NS_BasicHit'"));
}

void AValkyrieSlash::OnParticleOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//PrintViewport(1.f, FColor::White, TEXT("ValkyrieSlash OverlapBegin"));

	FActorSpawnParameters	SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AParticleNiagara* Particle =
		GetWorld()->SpawnActor<AParticleNiagara>(
			OtherActor->GetActorLocation(),
			FRotator::ZeroRotator,
			SpawnParam);

	Particle->SetParticle(TEXT("NiagaraSystem'/Game/sA_StylizedAttacksPack/FX/NiagaraSystems/NS_BasicHit.NS_BasicHit'"));


	if (IHitInterface* hitInterface = Cast<IHitInterface>(OtherActor))
	{
		hitInterface->GetHit(SweepResult.ImpactPoint);
	}
}

void AValkyrieSlash::OnSystemFinish(UNiagaraComponent* PSystem)
{
	mCollisionActor->Destroy();
	Destroy();
}

void AValkyrieSlash::SetDirection(const FVector& _value)
{
	mDirection = _value;
	mDirection.Normalize();
}

void AValkyrieSlash::Stop()
{
	mParticle->SetPaused(true);
	//GetWorld()->GetTimerManager().SetTimer(mTimer, this, &AValkyrieSlash::Start, 0.2f, false);
}

void AValkyrieSlash::Start()
{
	mParticle->ResetSystem();
}

void AValkyrieSlash::Initialize()
{
	if (mSlashIdx == 0)
	{
		mCollisionActor->SetCollisionShape(ECollisionType::ECS_Box);
		mCollisionActor->SetBoxExtent(FVector(32.f, 32.f, 32.f));
		mCollisionActor->SetRelativeLoctaion(FVector(100.f, 0.f, 450.f));
		mCollisionActor->SetRelativeScale(FVector(2.f, 2.f, 10.f));
		mCollisionActor->SetRelativeRotation(mDirection.Rotation());
		mCollisionActor->SetHiddenInGame(false);
	}
	else if (mSlashIdx == 1)
	{
		mCollisionActor->SetCollisionShape(ECollisionType::ECS_Box);
		mCollisionActor->SetBoxExtent(FVector(50.f, 50.f, 50.f));
		mCollisionActor->SetRelativeScale(FVector(1.f, 4.f, 1.f));
		mCollisionActor->SetRelativeRotation(mDirection.Rotation());
		mCollisionActor->SetHiddenInGame(false);
		
	}
	else if (mSlashIdx == 2)
	{
		mCollisionActor->SetCollisionShape(ECollisionType::ECS_Capsule);
		mCollisionActor->SetCapsuleHalfHeight(25.f);
		mCollisionActor->SetCapsuleRadius(10.f);
		mCollisionActor->SetRelativeScale(FVector(5.f, 5.f, 5.f));
		mCollisionActor->SetRelativeRotation(mDirection.Rotation());
		mCollisionActor->SetHiddenInGame(false);
	}
	else if (mSlashIdx == 3)
	{
		mCollisionActor->SetCollisionShape(ECollisionType::ECS_Sphere);
		mCollisionActor->SetSphereRadius(60.f);
		mCollisionActor->SetRelativeScale(FVector(2.f, 2.f, 2.f));
		mCollisionActor->SetRelativeRotation(mDirection.Rotation());
		mCollisionActor->SetHiddenInGame(false);
	}
}
