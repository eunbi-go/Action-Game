// Fill out your copyright notice in the Description page of Project Settings.


#include "ValkyrieContinuousSlash.h"
#include "../../Player/Valkyrie.h"
#include "../../Particle/ValkyrieSlash.h"

AValkyrieContinuousSlash::AValkyrieContinuousSlash()
{
}

void AValkyrieContinuousSlash::Activate()
{
	if (AValkyrie* player = Cast<AValkyrie>(mOwner))
	{
		player->SetMp(-10.f);
		player->SetActionState(EActionState::EAS_Attack_Skill, true);
		player->SetSkillState(ESkillState::ESS_Slash);
	}
}

void AValkyrieContinuousSlash::BeginPlay()
{
	Super::BeginPlay();
}

void AValkyrieContinuousSlash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AValkyrieContinuousSlash::SpawnEffect()
{
	FActorSpawnParameters	SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector location = mOwner->GetActorLocation();
	FRotator rotation = mOwner->GetActorRotation();

	if (mSlashSkillIndex == 0)
	{
		AValkyrieSlash* slash = GetWorld()->SpawnActor<AValkyrieSlash>(
			location,
			rotation,
			SpawnParam
		);
		slash->SetParticle(TEXT("NiagaraSystem'/Game/NiagaraMagicalSlashes/Fx/Slashes/NS_Col_SL_12.NS_Col_SL_12'"));
		slash->SetDirection(mOwner->GetActorForwardVector());
		slash->SetSlashIdx(0);
	}
	else if (mSlashSkillIndex == 1)
	{
		AValkyrieSlash* slash = GetWorld()->SpawnActor<AValkyrieSlash>(
			location,
			rotation,
			SpawnParam
		);
		slash->SetParticle(TEXT("NiagaraSystem'/Game/NiagaraMagicalSlashes/Fx/Slashes/NS_Col_SL_13.NS_Col_SL_13'"));
		slash->SetDirection(mOwner->GetActorForwardVector());
		slash->SetSlashIdx(1);
	}
	else if (mSlashSkillIndex == 2)
	{
		AValkyrieSlash* slash = GetWorld()->SpawnActor<AValkyrieSlash>(
			location,
			rotation,
			SpawnParam
		);
		slash->SetParticle(TEXT("NiagaraSystem'/Game/NiagaraMagicalSlashes/Fx/Slashes/NS_Blade_Sl_10.NS_Blade_Sl_10'"));
		slash->SetDirection(mOwner->GetActorForwardVector());
		slash->SetSlashIdx(2);
	}
	
	else if (mSlashSkillIndex == 3)
	{
		FVector forward = mOwner->GetActorForwardVector();
		forward.Y = 0.f;
		forward.Z = 0.f;
		AValkyrieSlash* slash = GetWorld()->SpawnActor<AValkyrieSlash>(
			location,
			FRotator(0.f, Cast<AValkyrie>(mOwner)->GetControlRotation().Yaw - 90.f, 0.f),
			SpawnParam
		);
		slash->SetActorScale3D(FVector(0.5f));
		slash->SetParticle(TEXT("NiagaraSystem'/Game/NiagaraMagicalSlashes/Fx/Slashes/NS_Slash_HeavyAb_03.NS_Slash_HeavyAb_03'"));
		slash->SetDirection(mOwner->GetActorForwardVector());
		slash->SetSlashIdx(3);
	}
}

void AValkyrieContinuousSlash::SkillEnd()
{
	mSlashSkillIndex = -1;
	mIsNextSlashEnable = false;
	mIsNextSlashInput = false;
	mIsSlash = false;

	if (AValkyrie* valkyrie = Cast<AValkyrie>(mOwner))
	{
		valkyrie->SetActionState(EActionState::EAS_Attack_Skill, false);
		valkyrie->SetSkillState(ESkillState::ESS_None);
	}

	Destroy();
}

void AValkyrieContinuousSlash::InputPressed()
{
	if (mIsSlash)
	{
		if (mIsNextSlashEnable)
			mIsNextSlashInput = true;
	}
	else
	{
		mIsSlash = true;

		mIsNextSlashEnable = true;
		mIsNextSlashInput = false;

		if (AValkyrie* valkyrie = Cast<AValkyrie>(mOwner))
		{
			valkyrie->PlayMontage(FName("Slash"), FName(*FString::Printf(TEXT("Slash%d"), ++mSlashSkillIndex)));
		}
		//PlayMontage(FName("Slash"), FName(*FString::Printf(TEXT("Slash%d"), ++mSlashSkillIndex)));
	}
}

void AValkyrieContinuousSlash::Notify_SlashEnable()
{
	mIsNextSlashEnable = false;
	if (mIsNextSlashInput)
	{

		mIsNextSlashEnable = true;
		mIsNextSlashInput = false;
		if (AValkyrie* valkyrie = Cast<AValkyrie>(mOwner))
		{
			valkyrie->PlayMontage(FName("Slash"), FName(*FString::Printf(TEXT("Slash%d"), ++mSlashSkillIndex)));
		}
		//PlayMontage(FName("Slash"), FName(*FString::Printf(TEXT("Slash%d"), ++mSlashSkillIndex)));
	}
}


