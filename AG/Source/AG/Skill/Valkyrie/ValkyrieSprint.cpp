// Fill out your copyright notice in the Description page of Project Settings.


#include "ValkyrieSprint.h"
//#include "../../Particle/ValkyrieLightning.h"
#include "../../Player/Valkyrie.h"
#include "../../Monster/Monster.h"
#include "../../Particle/ParticleNiagara.h"
#include "../../Collision/CollisionActor.h"

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
	FActorSpawnParameters	spawnParam;
	spawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector location = FVector();

	location = mOwner->GetActorLocation();
	location.Z -= Cast<AValkyrie>(mOwner)->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	AParticleNiagara* niagara = GetWorld()->SpawnActor<AParticleNiagara>(
		location,
		FRotator::ZeroRotator,
		spawnParam
	);
	niagara->SetParticle(TEXT("NiagaraSystem'/Game/NiagaraMagicalSlashes/Fx/Slashes/NS_SlashRing_05.NS_SlashRing_05'"));

	mCollisionActor = GetWorld()->SpawnActor<ACollisionActor>(
		location,
		FRotator::ZeroRotator,
		spawnParam
	);
	mCollisionActor->SetOwnerActor(niagara);
	mCollisionActor->SetCollisionProfileName(FName("PlayerSword"));
	mCollisionActor->SetParent(this);
	mCollisionActor->SetCollisionShape(ECollisionType::ECS_Sphere);
	mCollisionActor->SetSphereRadius(60.f);
	mCollisionActor->SetRelativeScale(FVector(4.f));
	//mCollisionActor->SetHiddenInGame(false);
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

	const FVector& owner_location = mOwner->GetActorLocation();

	if (isOverlapped)
	{
		float distance = 5000.f;
		int32 actors_cnt = outActors.Num(), i = 0;
		AActor* target = nullptr;
		for (i = 0; i < actors_cnt; ++i)
		{
			if (IsValid(Cast<AMonster>(outActors[i])))
			{
				float dis = GetDistanceTo(outActors[i]);
				if (dis <= distance)
				{
					distance = dis;
					target = outActors[i];
				}
			}
		}

		if (AValkyrie* valkyrie = Cast<AValkyrie>(mOwner))
		{
			valkyrie->SetMotionWarpingComponent(target->GetActorLocation());
		}
	}
}

void AValkyrieSprint::SkillEnd()
{
	if (AValkyrie* valkyrie = Cast<AValkyrie>(mOwner))
	{
		valkyrie->SetSkillState(ESkillState::ESS_None);
		valkyrie->SetActionState(EActionState::EAS_Attack_Skill, false);
	}
	mCollisionActor->Destroy();
	Destroy();
}