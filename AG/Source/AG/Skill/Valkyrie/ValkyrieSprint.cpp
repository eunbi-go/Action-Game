// Fill out your copyright notice in the Description page of Project Settings.


#include "ValkyrieSprint.h"
#include "../../Particle/ValkyrieLightning.h"
#include "../../Player/Valkyrie.h"
#include "../../Monster/Monster.h"

AValkyrieSprint::AValkyrieSprint()
{
}

void AValkyrieSprint::Activate()
{
	if (AValkyrie* valkyrie = Cast<AValkyrie>(mOwner))
	{
		valkyrie->SetMp(-10.f);
		valkyrie->SetSkillState(ESkillState::ESS_Sprint);
		valkyrie->SetActionState(EActionState::EAS_Attack_Skill, true);
	}
}

void AValkyrieSprint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AValkyrieSprint::BeginPlay()
{
	Super::BeginPlay();
}

void AValkyrieSprint::SpawnEffect()
{
	FActorSpawnParameters	SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector location = FVector();

	location = mOwner->GetActorLocation();
	location.Z -= Cast<AValkyrie>(mOwner)->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	AValkyrieLightning* niagara = GetWorld()->SpawnActor<AValkyrieLightning>(
		location,
		FRotator::ZeroRotator,
		SpawnParam
	);
	niagara->SetParticle(TEXT("NiagaraSystem'/Game/NiagaraMagicalSlashes/Fx/Slashes/NS_SlashRing_05.NS_SlashRing_05'"));
}

void AValkyrieSprint::FindTarget()
{
	FVector location = GetActorLocation();
	FVector targetLocation = FVector();
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	TArray<AActor*> ignoreActors;
	TArray<AActor*> outActors;

	objectTypes.Add(UEngineTypes::ConvertToObjectType(
		ECollisionChannel::ECC_GameTraceChannel3)
	);
	ignoreActors.Add(this);
	ignoreActors.Add(mOwner);

	bool isOverlapped = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		location,
		5000.f,
		objectTypes,
		nullptr,
		ignoreActors,
		outActors
	);

	if (isOverlapped)
	{
		if (IsValid(Cast<AMonster>(outActors[0])))
		{
			mTargetLocation = outActors[0]->GetActorLocation();
		}
	}

	if (AValkyrie* valkyrie = Cast<AValkyrie>(mOwner))
	{
		valkyrie->SetMotionWarpingComponent(mTargetLocation);
	}
}

void AValkyrieSprint::SkillEnd()
{
	if (AValkyrie* valkyrie = Cast<AValkyrie>(mOwner))
	{
		valkyrie->SetSkillState(ESkillState::ESS_None);
		valkyrie->SetActionState(EActionState::EAS_Attack_Skill, false);
	}

	Destroy();
}
