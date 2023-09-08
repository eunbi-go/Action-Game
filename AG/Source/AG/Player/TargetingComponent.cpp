#include "TargetingComponent.h"
#include "../Monster/Monster.h"

UTargetingComponent::UTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UTargetingComponent::LockingCamera(float DeltaTime)
{
	FRotator rotation = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetControlRotation();
	FRotator interpDstRotator;
	interpDstRotator.Roll = 0.f;
	interpDstRotator.Pitch =
		UKismetMathLibrary::MapRangeClamped(mTarget->GetDistanceTo(mOwner), 1000.f, 150.f, 0.f, -35.f);
	interpDstRotator.Yaw =
		UKismetMathLibrary::FindLookAtRotation(mOwner->GetActorLocation(), mTarget->GetActorLocation()).Yaw;
	FRotator newRotator = 
		FMath::RInterpTo(rotation, interpDstRotator, DeltaTime, 6.f);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->
		SetControlRotation(FRotator(newRotator.Pitch, newRotator.Yaw, rotation.Roll));
}


void UTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (mIsTargetLock)
	{

		if (mTarget && mTarget->GetIsDead())
		{
			mTarget = nullptr;
			CheckTarget();
			if (!mTarget)
			{
				mOwner->bUseControllerRotationYaw = false;
				mOwner->GetCharacterMovement()->bOrientRotationToMovement = true;
				mIsTargetLock = false;
				return;
			}
		}

		if (mTarget)
			LockingCamera(DeltaTime);
		else
			CheckTarget();
	}
}

void UTargetingComponent::CheckTarget()
{
	FVector location = mOwner->GetActorLocation();
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	TArray<AActor*> ignoreActors;
	TArray<AActor*> outActors;
	//AActor* target;

	objectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3));
	ignoreActors.Add(mOwner);

	bool isOverlapped = UKismetSystemLibrary::SphereOverlapActors(
							GetWorld(),
							location,
							5000.f,	// sphere radius
							objectTypes,
							nullptr,
							ignoreActors,
							outActors
						);

	if (isOverlapped)
	{
		mTarget = Cast<AMonster>(GetClosetActor(outActors, FName("Monster")));
		if (mTarget)
		{
			//PrintViewport(1.f, FColor::Black, FString("Find Character Target"));
			mIsTargetLock = true;
			mIsYaw = true;
			mIsControl = false;
			mOwner->bUseControllerRotationYaw = true;
			mOwner->GetCharacterMovement()->bOrientRotationToMovement = false;
			mOwner->SetActorTickEnabled(false);
		}
		else
		{
			//PrintViewport(1.f, FColor::Red, FString("Target Unlock"));
			mIsTargetLock = false;
			mOwner->bUseControllerRotationYaw = false;
			mOwner->GetCharacterMovement()->bOrientRotationToMovement = true;
			mOwner->SetActorTickEnabled(true);
		}
	}
}

AActor* UTargetingComponent::GetClosetActor(TArray<AActor*> actors, FName targetTag)
{
	int num = actors.Num();
	float distance = 10000.f;
	AActor* closetActor = nullptr;
	TArray<AActor*> ignoreActors;
	ignoreActors.Add(mOwner);
	for (int i = 0; i < num; ++i)
	{
		FHitResult res;
		UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			actors[i]->GetActorLocation(),
			mOwner->GetActorLocation(),
			ETraceTypeQuery::TraceTypeQuery1,	// Visibility
			false,
			ignoreActors,
			EDrawDebugTrace::ForDuration,
			res,
			true
		);

		if (res.GetActor())
		{
			float dis = res.GetActor()->GetDistanceTo(mOwner);
			if (dis < distance)
			{
				PrintViewport(1.f, FColor::Black, FString("Find"));
				distance = dis;
				closetActor = res.GetActor();
			}
		}
	}

	return closetActor;
}

void UTargetingComponent::SetTargetLock()
{
	mIsTargetLock = !mIsTargetLock;

	if (mIsTargetLock)
	{
		CheckTarget();
	}
	else
	{
		mOwner->bUseControllerRotationYaw = false;
		mOwner->GetCharacterMovement()->bOrientRotationToMovement = true;
		mOwner->SetActorTickEnabled(true);
	}
}

