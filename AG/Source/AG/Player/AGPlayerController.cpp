// Fill out your copyright notice in the Description page of Project Settings.


#include "AGPlayerController.h"
#include "../Particle/Decal.h"
#include "../Interface/EnemyInterface.h"

AAGPlayerController::AAGPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	bShowMouseCursor = true;

	mPickingPosition = FVector(0.f, 0.f, 0.f);
}

void AAGPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	TraceCursor();
}

void AAGPlayerController::BeginPlay()
{
	Super::BeginPlay();



	//FInputModeGameOnly	Mode;
	FInputModeGameAndUI	Mode;
	Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	Mode.SetHideCursorDuringCapture(false);
	SetInputMode(Mode);

	mMousePick = GetWorld()->SpawnActor<ADecal>(FVector::ZeroVector,
		FRotator::ZeroRotator);

	mMousePick->SetDecalMaterial(TEXT("Material'/Game/MTMagicCircle.MTMagicCircle'"));
	mMousePick->SetDecalVisibility(false);
}

void AAGPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AAGPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void AAGPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AAGPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAGPlayerController::SpawnDecalOnMousePick()
{
	//FHitResult	result;
	//bool Hit = GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1,
	//	false, result);

	//if (Hit)
	//{
	//		mPickActor = result.GetActor();
	//		mMousePick->SetActorLocation(result.ImpactPoint);

	//		FActorSpawnParameters	SpawnParam;
	//		SpawnParam.SpawnCollisionHandlingOverride =
	//			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//		mPickingPosition = mMousePick->GetActorLocation();

	//		ADecal* Decal =
	//			GetWorld()->SpawnActor<ADecal>(
	//				mPickingPosition,
	//				FRotator(0.f, 0.f, 0.f),
	//				SpawnParam);

	//		Decal->SetActorScale3D(FVector(0.2f, 0.2f, 0.2f));
	//		Decal->SetDecalMaterial(TEXT("Material'/Game/MTMagicCircle_2.MTMagicCircle_2'"));
	//		Decal->SetLifeSpan(5.f);
	//		Decal->SetDecalVisibility(true);
	//}
}

void AAGPlayerController::SetInputModeType(INPUT_MODE_TYPE _type)
{
	/*switch (_type)
	{
	case INPUT_MODE_TYPE::GAME_ONLY:
	{
		FInputModeGameOnly	Mode;
		SetInputMode(Mode);
	}
	break;

	case INPUT_MODE_TYPE::GAME_UI:
	{
		FInputModeGameAndUI	Mode;
		SetInputMode(Mode);
	}
	break;
	}*/
}

void AAGPlayerController::TraceCursor()
{
	FHitResult hitResult;
	GetHitResultUnderCursor(ECC_Visibility, false, hitResult);
	if (!hitResult.bBlockingHit) return;

	mPreActor = mCurActor;
	mCurActor = Cast<IEnemyInterface>(hitResult.GetActor());

	if (!mPreActor)
	{
		if (mCurActor)
		{
			mCurActor->HighlightActor();
		}
		else
		{

		}
	}
	else
	{
		if (!mCurActor)
		{
			mPreActor->UnHighlightActor();
		}
		else
		{
			if (mPreActor != mCurActor)
			{
				mPreActor->UnHighlightActor();
				mCurActor->HighlightActor();
			}
			else
			{
				// 동일한 액터
			}
		}
	}
}
