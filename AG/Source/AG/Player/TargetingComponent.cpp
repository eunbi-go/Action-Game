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
	//PrintViewport(1.f, FColor::Black, FString("Lockinggggggg"));
	FRotator rot = owner->GetController()->GetControlRotation();
	FRotator targetRot = UKismetMathLibrary::FindLookAtRotation(owner->GetActorLocation(), mTarget->GetActorLocation());
	FRotator interpRot = UKismetMathLibrary::RInterpTo(rot, targetRot, DeltaTime, 2.f);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetControlRotation(FRotator(interpRot.Pitch, interpRot.Yaw, rot.Roll));

	/*FRotator rot = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetControlRotation();
	FRotator dstRotator;
	dstRotator.Roll = 0.f;
	dstRotator.Pitch = UKismetMathLibrary::MapRangeClamped(mTarget->GetDistanceTo(owner), 1500.f, 150.f, 0.f, -35.f);
	dstRotator.Yaw = UKismetMathLibrary::FindLookAtRotation(owner->GetActorLocation(), mTarget->GetActorLocation()).Yaw;
	FRotator newRotator = FMath::RInterpTo(rot, dstRotator, DeltaTime, 6.f);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetControlRotation(FRotator(newRotator.Pitch, newRotator.Yaw, rot.Roll));*/
}


void UTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (mIsLock)
	{
		LockingCamera(DeltaTime);
	}
}

void UTargetingComponent::CheckTarget()
{
	FVector location;
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	TArray<AActor*> ignoreActors;
	TArray<AActor*> outActors;
	//AActor* target;

	objectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3));
	ignoreActors.Add(owner);

	bool isOverlapped = UKismetSystemLibrary::SphereOverlapActors(
							GetWorld(),
							location,
							3000.f,	// sphere radius
							objectTypes,
							nullptr,
							ignoreActors,
							outActors
						);

	if (isOverlapped)
	{
		mTarget = Cast<ACharacter>(GetClosetActor(outActors, FName("Monster")));
		if (mTarget)
		{
			PrintViewport(1.f, FColor::Black, FString("Find Character Target"));
			mIsLock = true;
			mIsYaw = true;
			mIsControl = false;
			owner->bUseControllerRotationYaw = true;
			owner->GetCharacterMovement()->bOrientRotationToMovement = false;
			owner->SetActorTickEnabled(false);
		}
	}
}

AActor* UTargetingComponent::GetClosetActor(TArray<AActor*> actors, FName targetTag)
{
	int num = actors.Num();
	float distance = 1000.f;
	AActor* closetActor = nullptr;
	TArray<AActor*> ignoreActors;
	ignoreActors.Add(owner);
	for (int i = 0; i < num; ++i)
	{
		FHitResult res;
		UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			actors[i]->GetActorLocation(),
			owner->GetActorLocation(),
			ETraceTypeQuery::TraceTypeQuery1,	// Visibility
			false,
			ignoreActors,
			EDrawDebugTrace::ForDuration,
			res,
			true
		);

		if (res.GetActor())
		{
			float dis = res.GetActor()->GetDistanceTo(owner);
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

