// Fill out your copyright notice in the Description page of Project Settings.


#include "RampageSlash.h"
#include "../Player/PlayerCharacter.h"

ARampageSlash::ARampageSlash()
{
	mIsEnableMove = false;

	static ConstructorHelpers::FClassFinder<UCameraShakeBase>	cameraShake(TEXT("Blueprint'/Game/Blueprints/CameraShake/CS_PlayerNormalAttack.CS_PlayerNormalAttack_C'"));

	if (cameraShake.Succeeded())
		mHitShake = cameraShake.Class;
}

void ARampageSlash::ResetSlash(AActor* preParticle)
{
	mParticle->ResetSystem();
}

void ARampageSlash::MoveStart(APlayerCharacter* target)
{
	mTarget = target;
	//targetPosition = target->GetActorLocation();
	mIsEnableMove = true;
	//mParticle->ResetSystem();
	//mParticle->SetCustomTimeDilation(0.5f);
}

void ARampageSlash::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(mTimerHandle2, this, &ARampageSlash::Stop, 0.2f, false);
}

void ARampageSlash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!mIsEnableMove)
		return;
	else
	{
		FVector position = GetActorLocation();
		FVector targetPosition = mTarget->GetActorLocation();
		FVector direction = targetPosition - position;

		FRotator targetRotation = FRotationMatrix::MakeFromX(direction.GetSafeNormal2D()).Rotator();

		SetActorRotation(FMath::RInterpTo(GetActorRotation(), targetRotation, DeltaTime, 2.f));


		// �̵�.
		float	Distance = FVector::Distance(position, targetPosition);
		Distance -= mTarget->GetCapsuleComponent()->GetScaledCapsuleRadius();

		SetActorLocation(position + direction.GetSafeNormal() * DeltaTime * 1500.0f);
		
		

		if (Distance <= 500.f)
			mParticle->ResetSystem();

		if (Distance <= 100.f)
		{
			// �浹�ߴٰ� ����.
			mOnHit.Broadcast(GetActorLocation());

			// ī�޶� ����ũ �߰�.
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(mHitShake);

			Destroy();
		}
	}
}

void ARampageSlash::Stop()
{
	mReSpawn.Broadcast(this);
	mParticle->SetPaused(true);
}
