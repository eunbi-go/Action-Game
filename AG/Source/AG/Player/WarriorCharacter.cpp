// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorCharacter.h"

#include "../Particle/ParticleCascade.h"
#include "../Basic/WeaponActor.h"
#include "../Skill/TeleportSkill.h"
#include "PlayerAnimInstance.h"
#include "AGPlayerController.h"
#include "../Basic/TemporaryfCameraActor.h"

AWarriorCharacter::AWarriorCharacter()
{
	//------------------
	// Mesh ����.
	//------------------
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>    characterAsset(TEXT("SkeletalMesh'/Game/Assets/Character/Valkyrie/Mesh/SK_Valkyrie.SK_Valkyrie'"));

	if (characterAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(characterAsset.Object);

	GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -90.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));




	//-----------------------------------------
	// AnimInstance ����.
	//-----------------------------------------
	static ConstructorHelpers::FClassFinder<UAnimInstance>	animInst(TEXT("AnimBlueprint'/Game/Blueprints/Player/Animation/ABP_Warrior.ABP_Warrior_C'"));
	
	if (animInst.Succeeded())
		GetMesh()->SetAnimInstanceClass(animInst.Class);




	//-----------------------------------------
	// CameraShake ����.
	//-----------------------------------------
	static ConstructorHelpers::FClassFinder<UCameraShakeBase>	cameraShake(TEXT("Blueprint'/Game/Blueprints/CameraShake/CS_PlayerNormalAttack.CS_PlayerNormalAttack_C'"));

	if (cameraShake.Succeeded())
		mNormalAttackShake = cameraShake.Class;



	


	mIsGaugeEnd = false;
}

void AWarriorCharacter::BeginPlay()
{
	Super::BeginPlay();

	//-----------------------------------------
	// ���� ����.
	//-----------------------------------------
	FActorSpawnParameters	SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	mWeapon = GetWorld()->SpawnActor<AWeaponActor>(
		AWeaponActor::StaticClass(), SpawnParam);

	mWeapon->SetSkeletalMesh(TEXT("SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_HeroSword11/SK_Blade_HeroSword11.SK_Blade_HeroSword11'"));


	mWeapon->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::KeepRelativeTransform,
		TEXT("thigh_l"));
	
	mWeapon->SetActorRelativeLocation(FVector(5.0f, 7.0f, -11.0f));
	mWeapon->SetActorRelativeRotation(FRotator(10.f, 151.f, 107.f));




	//---------------------------
	// Skill ���� ����.
	//---------------------------
	FSkillInfo skillInfo;
	skillInfo.slotNumber = 0;
	skillInfo.skillType = SKILL_TYPE::TELEPORT;
	skillInfo.minDamage = 100;
	skillInfo.maxDamage = 200;

	ATeleportSkill* skillProjectile = NewObject<ATeleportSkill>(this,
		ATeleportSkill::StaticClass());
	skillInfo.skillActor = Cast<ASkillActor>(skillProjectile);

	//SkillProjectile->mOnSkillEnd.AddDynamic(this,
	//	&AKnightCharacter::Skill1End);

	mSkillInfoArray.Add(skillInfo);
}

void AWarriorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (mAnimInst->GetCurSkillType() == SKILL_TYPE::TELEPORT && mIsGaugeEnd)
	{
		FVector pos = GetActorLocation();
		FVector dest = Cast<AAGPlayerController>(GetController())->GetPickingPosition();
		FVector direction = dest - pos;

		float distance = direction.Length() + GetCapsuleComponent()->GetScaledCapsuleRadius() * 2.f;
		direction = direction.GetSafeNormal();

		SetActorLocation(pos + direction * DeltaTime * 1000.f);
		if (FVector::Distance(pos, dest) <= 100.f)
		{
			mIsGaugeEnd = false;

			StopLaunchCharacter();
			PrintViewport(10.f, FColor::Red, TEXT("FINISH"));
		}
	}
}

void AWarriorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void AWarriorCharacter::NormalAttackCheck()
{
	if (mAnimInst->GetNormalAttackIndex() == 3)
	{
		return;
	}
	
	else
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(mNormalAttackShake);
	
}

void AWarriorCharacter::Skill1()
{
	SKILL_TYPE	skillType;
	int32	count = mSkillInfoArray.Num();

	for (int32 i = 0; i < count; ++i)
	{
		if (mSkillInfoArray[i].slotNumber == 0)
		{
			skillType = mSkillInfoArray[i].skillType;
			break;
		}
	}

	if (skillType == SKILL_TYPE::SKILL_TYPE_END)
		return;

	mAnimInst->UseSkill(skillType);
}

void AWarriorCharacter::Skill2()
{
}

void AWarriorCharacter::UseSkill(SKILL_TYPE _skillType)
{
	int32	count = mSkillInfoArray.Num();

	for (int32 i = 0; i < count; ++i)
	{
		if (mSkillInfoArray[i].skillType == _skillType)
		{
			SpawnSkill(_skillType, i);
			break;
		}
	}
}

void AWarriorCharacter::SpawnSkill(SKILL_TYPE _skillType, int32 _skillInfoArrayIndex)
{
	FActorSpawnParameters	SpawnParam;
	SpawnParam.Template = mSkillInfoArray[_skillInfoArrayIndex].skillActor;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	switch (mAnimInst->GetCurSkillType())
	{
	case SKILL_TYPE::TELEPORT:
	{
		ATeleportSkill* Skill =
			GetWorld()->SpawnActor<ATeleportSkill>(
				GetActorLocation() + GetActorForwardVector() * 100.f,
				GetActorRotation(),
				SpawnParam);
	}
	break;
	}
}

void AWarriorCharacter::Gauge(float _scale)
{
	if (_scale == 0.f || mAnimInst->GetPlayerMotion() != PLAYER_MOTION::SKILL)
		return;

	switch (mAnimInst->GetCurSkillType())
	{
	case SKILL_TYPE::TELEPORT:
		mTeleportGauge += GetWorld()->GetDeltaSeconds();
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(mGaugeShake);
		mWeapon->IsGaugeOn(true);
		break;
	}
}

void AWarriorCharacter::GaugeEnd()
{
	switch (mAnimInst->GetCurSkillType())
	{
	case SKILL_TYPE::TELEPORT:
	{
		//---------------------
		// ī�޶� ����ŷ�� ������, �÷��̾ �����̵���Ų��.
		//---------------------

		mIsGaugeEnd = true;

		GetWorld()->GetFirstPlayerController()->ClientStopCameraShake(mGaugeShake);

		// 1. mSpringArm ��Ȱ��ȭ.
		mSpringArm->SetActive(false);

		// 2. ī�޶� ���� ����.
		FActorSpawnParameters	SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride =	ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


		mTempCamera = GetWorld()->SpawnActor<ATemporaryfCameraActor>(
			ATemporaryfCameraActor::StaticClass(), SpawnParam);

		GetWorld()->GetFirstPlayerController()->SetViewTarget(mTempCamera);

		//mTempCamera->SetRatio(mCamera->AspectRatio);
		mTempCamera->GetCameraComponent()->SetAspectRatio(mCamera->AspectRatio);

		FVector pos = GetActorLocation();
		pos.X -= GetActorForwardVector().X * 400.f;
		pos.Z += 200.f;

		mTempCamera->SetActorLocation(pos);
	}
	break;
	}
}

void AWarriorCharacter::StopLaunchCharacter()
{
	PrintViewport(10.f, FColor::Red, TEXT("StopLaunchCharacter()"));

	switch (mAnimInst->GetCurSkillType())
	{
	case SKILL_TYPE::TELEPORT:
	{
		//---------------------
		// �����̵��� ��, ���� �ֵθ��� �ִϸ��̼����� ��� ��ġ�� �ٲٰ� 
		// 0.5�� ���� ��� �����ִٰ� �ٽ� �ִϸ��̼��� ����Ѵ�.
		//---------------------

		mTempCamera->Destroy();
		mSpringArm->SetActive(true);

		//CustomTimeDilation = 1.f;
		//mSpringArm->bEnableCameraLag = false;
		//mSpringArm->bEnableCameraRotationLag = false;
		//mSpringArm->CameraLagSpeed = 5.f;

		PrintViewport(10.f, FColor::Red, TEXT("StopLaunchCharacter()"));
		mAnimInst->GaugeEnd();
		GetWorldTimerManager().SetTimer(timerHandle, this, &APlayerCharacter::RestartSkill, 0.2f, false);
	}
	break;
	}

	
}

void AWarriorCharacter::RestartSkill()
{
	switch (mAnimInst->GetCurSkillType())
	{
	case SKILL_TYPE::TELEPORT:
	{
		//---------------------
		// ����� �ִϸ��̼��� ����Ѵ�. Teleport ��ų ��.
		//---------------------

		mAnimInst->RestartSkill();
		mWeapon->IsGaugeOn(false);
		mWeapon->SetTrailOnOff(true);

		//mCamera->SetActive(true);
		mSpringArm->CameraLagSpeed = 8.f;
		mSpringArm->bEnableCameraLag = false;
	}
	break;
	}
	
}
