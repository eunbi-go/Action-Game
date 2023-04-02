// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorCharacter.h"

#include "../Particle/ParticleCascade.h"
#include "../Basic/WeaponActor.h"
#include "../Skill/TeleportSkill.h"
#include "PlayerAnimInstance.h"
#include "AGPlayerController.h"
#include "../Basic/TemporaryfCameraActor.h"
#include "../Skill/SprintSkil.h"

AWarriorCharacter::AWarriorCharacter()
{
	//------------------
	// Mesh 세팅.
	//------------------
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>    characterAsset(TEXT("SkeletalMesh'/Game/Assets/Character/Valkyrie/Mesh/SK_Valkyrie.SK_Valkyrie'"));

	if (characterAsset.Succeeded())
		GetMesh()->SetSkeletalMesh(characterAsset.Object);

	GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -90.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));




	//-----------------------------------------
	// AnimInstance 지정.
	//-----------------------------------------
	static ConstructorHelpers::FClassFinder<UAnimInstance>	animInst(TEXT("AnimBlueprint'/Game/Blueprints/Player/Animation/ABP_Warrior.ABP_Warrior_C'"));
	
	if (animInst.Succeeded())
		GetMesh()->SetAnimInstanceClass(animInst.Class);




	//-----------------------------------------
	// CameraShake 지정.
	//-----------------------------------------
	static ConstructorHelpers::FClassFinder<UCameraShakeBase>	cameraShake(TEXT("Blueprint'/Game/Blueprints/CameraShake/CS_PlayerNormalAttack.CS_PlayerNormalAttack_C'"));

	if (cameraShake.Succeeded())
		mNormalAttackShake = cameraShake.Class;



	
	//-----------------------------------------
	// 기타 멤버변수 초기화.
	//-----------------------------------------
	mIsGaugeEnd = false;
	mSprintCount = 0;
	mSprintDirection = FVector(0.f, 0.f, 0.f);
	isSprint = false;

	mContinuousTime = 0.f;
}

void AWarriorCharacter::BeginPlay()
{
	Super::BeginPlay();

	//-----------------------------------------
	// 무기 장착.
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
	// Skill 정보 세팅.
	//---------------------------

	// 1. TELEPORT
	FSkillInfo skillInfo{};
	skillInfo.slotNumber = 0;
	skillInfo.skillType = SKILL_TYPE::TELEPORT;
	skillInfo.minDamage = 100;
	skillInfo.maxDamage = 200;

	ATeleportSkill* skillActor = NewObject<ATeleportSkill>(this,
		ATeleportSkill::StaticClass());
	skillInfo.skillActor = Cast<ASkillActor>(skillActor);

	//SkillProjectile->mOnSkillEnd.AddDynamic(this,
	//	&AKnightCharacter::Skill1End);

	mSkillInfoArray.Add(skillInfo);


	// 2. TELEPORT
	FSkillInfo skillInfo2{};
	skillInfo2.slotNumber = 1;
	skillInfo2.skillType = SKILL_TYPE::SPRINT;
	skillInfo2.minDamage = 300;
	skillInfo2.maxDamage = 700;

	ASprintSkil* skillActor2 = NewObject<ASprintSkil>(this,
		ASprintSkil::StaticClass());
	skillInfo2.skillActor = Cast<ASkillActor>(skillActor2);

	mSkillInfoArray.Add(skillInfo2);

	// 3. CONTINUOUS
	FSkillInfo skillInfo3{};
	skillInfo3.slotNumber = 2;
	skillInfo3.skillType = SKILL_TYPE::CONTINUOUS;
	skillInfo3.minDamage = 300;
	skillInfo3.maxDamage = 700;

	ASprintSkil* skillActor3 = NewObject<ASprintSkil>(this,
		ASprintSkil::StaticClass());
	skillInfo3.skillActor = Cast<ASkillActor>(skillActor3);

	mSkillInfoArray.Add(skillInfo3);

}

void AWarriorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (mAnimInst->GetCurSkillType())
	{
	case SKILL_TYPE::TELEPORT:
	{
		if (mIsGaugeEnd)
		{
			FVector pos = GetActorLocation();
			FVector dest = Cast<AAGPlayerController>(GetController())->GetPickingPosition();
			
			// 목표 지점으로 회전.
			FRotator targetRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),
				dest);

			FRotator rotator = GetActorRotation();

			rotator = FMath::RInterpTo(rotator, targetRot, DeltaTime, 1000.f);

			SetActorRotation(rotator);

			// 이동.
			CustomTimeDilation = 3.f;
			FVector direction = (dest - pos).GetSafeNormal();

			SetActorLocation(pos + direction * DeltaTime * 1000.f);

			if (FVector::Distance(pos, dest) <= 100.f)
			{
				CustomTimeDilation = 1.f;
				mIsGaugeEnd = false;

				StopLaunchCharacter();
				PrintViewport(10.f, FColor::Red, TEXT("FINISH"));
			}
		}
	}
	break;


	case SKILL_TYPE::SPRINT:
	{
		if (mSprintCount == 0 && !isSprint)
			mSprintDirection = FVector(1.f, 1.f, 0.f);

		else if (mSprintCount == 1 && !isSprint)
			mSprintDirection = FVector(-1.f, 0.f, 0.f);

		else if (mSprintCount == 2 && !isSprint)
			mSprintDirection = FVector(1.f, -1.f, 0.f);

		else if (mSprintCount == 3 && !isSprint)
			mSprintDirection = FVector(-1.f, 0.f, 0.f);

		if (mSprintCount <= 3 && !isSprint)
		{
			isSprint = true;
			GetCharacterMovement()->BrakingFrictionFactor = 0.f;

			// 목표 지점으로 회전.
			FRotator targetRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),
				GetActorLocation() + mSprintDirection * dist);

			FRotator rotator = GetActorRotation();

			rotator = FMath::RInterpTo(rotator, targetRot, DeltaTime, 1000.f);

			SetActorRotation(rotator);

			// 이동.
			LaunchCharacter(mSprintDirection * dist, true, true);
			GetWorldTimerManager().SetTimer(timerHandle, this, &AWarriorCharacter::NextSprint, 0.2f, false);
		}
	}
	break;


	case SKILL_TYPE::CONTINUOUS:
	{
		GetCharacterMovement()->StopMovementImmediately();

		if (GetActorLocation().Z >= 600.f)
		{
			GetCharacterMovement()->StopMovementImmediately();
			GetCharacterMovement()->BrakingFrictionFactor = 2.f;
		}

		mContinuousTime += DeltaTime;

		if (mContinuousTime >= 3.f)
		{
			mContinuousTime = 0.f;
			mAnimInst->StopContinuousSkill();
		}
	}
	break;


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
	SKILL_TYPE	skillType = SKILL_TYPE::SKILL_TYPE_END;
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
	SKILL_TYPE	skillType = SKILL_TYPE::SKILL_TYPE_END;
	int32	count = mSkillInfoArray.Num();

	for (int32 i = 0; i < count; ++i)
	{
		if (mSkillInfoArray[i].slotNumber == 1)
		{
			skillType = mSkillInfoArray[i].skillType;
			break;
		}
	}

	if (skillType == SKILL_TYPE::SKILL_TYPE_END)
		return;

	mAnimInst->UseSkill(skillType);
	UseSkill(skillType);

}

void AWarriorCharacter::Skill3()
{
	SKILL_TYPE	skillType = SKILL_TYPE::SKILL_TYPE_END;
	int32	count = mSkillInfoArray.Num();

	for (int32 i = 0; i < count; ++i)
	{
		if (mSkillInfoArray[i].slotNumber == 2)
		{
			skillType = mSkillInfoArray[i].skillType;
			break;
		}
	}

	if (skillType == SKILL_TYPE::SKILL_TYPE_END)
		return;

	mAnimInst->UseSkill(skillType);
	UseSkill(skillType);
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

	switch (_skillType)
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

	case SKILL_TYPE::SPRINT:
	{
		GetCharacterMovement()->AirControl = 0.2f;
		GetCharacterMovement()->BrakingFrictionFactor = 0.f;
		CustomTimeDilation = 2.f;

		//ASprintSkil* Skill =
		//	GetWorld()->SpawnActor<ASprintSkil>(
		//		GetActorLocation() + GetActorForwardVector() * 100.f,
		//		GetActorRotation(),
		//		SpawnParam);

		//TempCameraOnOff(true);
	}
	break;

	case SKILL_TYPE::CONTINUOUS:
	{
		//GetCharacterMovement()->AirControl = 0.2f;
		//GetCharacterMovement()->BrakingFrictionFactor = 0.f;

		//LaunchCharacter(FVector(0.f, 0.f, 1.f) * 4000.f, true, true);
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
		// 카메라 세이킹을 끝내고, 플레이어를 순간이동시킨다.
		//---------------------

		mIsGaugeEnd = true;

		GetWorld()->GetFirstPlayerController()->ClientStopCameraShake(mGaugeShake);

		// 1. mSpringArm 비활성화.
		

		// 2. 카메라 액터 스폰.
		FActorSpawnParameters	SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride =	ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


		mTempCamera = GetWorld()->SpawnActor<ATemporaryfCameraActor>(
			ATemporaryfCameraActor::StaticClass(), SpawnParam);

		GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(mTempCamera, 0.7f);
		//mTempCamera->SetCamera(mCamera);
		mTempCamera->SetSpringArm(mSpringArm);

		mTempCamera->SetRatio(mCamera->AspectRatio);
		mTempCamera->GetCameraComponent()->SetAspectRatio(1.777778);

		mSpringArm->SetActive(false);
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
	switch (mAnimInst->GetCurSkillType())
	{
	case SKILL_TYPE::TELEPORT:
	{
		//---------------------
		// 순간이동한 후, 검을 휘두르는 애니메이션으로 재생 위치를 바꾸고 
		// 0.5초 동안 잠시 멈춰있다가 다시 애니메이션을 재생한다.
		//---------------------

		
		GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(this, 0.7f);
		mTempCamera->Destroy();
		mSpringArm->SetActive(true);

		//CustomTimeDilation = 1.f;
		//mSpringArm->bEnableCameraLag = false;
		//mSpringArm->bEnableCameraRotationLag = false;
		//mSpringArm->CameraLagSpeed = 5.f;

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
		// 멈췄던 애니메이션을 재생한다. Teleport 스킬 끝.
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

void AWarriorCharacter::NextSprint()
{
	GetCharacterMovement()->StopMovementImmediately();
	++mSprintCount;
	

	// 마지막 점프 공격.
	if (mSprintCount == 5)
	{
		PrintViewport(10.f, FColor::Black, TEXT("NextSprint() last"));
		mAnimInst->ReplaySprintMontage();
	}

	// 이동 공격.
	else if (mSprintCount < 5)
		mAnimInst->ResumSprintAttackMontage();

	// 스킬 끝.
	else
	{
		PrintViewport(10.f, FColor::Black, TEXT("NextSprint() end"));
		GetCharacterMovement()->BrakingFrictionFactor = 2.f;
		isSprint = false;
		mSprintCount = 0;
		CustomTimeDilation = 1.f;
		mAnimInst->SetIsPlayerMotion(PLAYER_MOTION::IDLE);
	}
}

void AWarriorCharacter::TempCameraOnOff(bool _value)
{
	// on.
	if (_value)
	{

		FActorSpawnParameters	SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


		mTempCamera = GetWorld()->SpawnActor<ATemporaryfCameraActor>(
			ATemporaryfCameraActor::StaticClass(), SpawnParam);

		GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(mTempCamera, 2.f);
		//mTempCamera->SetCamera(mCamera);
		mTempCamera->SetSpringArm(mSpringArm);

		mTempCamera->SetRatio(mCamera->AspectRatio);
		mTempCamera->GetCameraComponent()->SetAspectRatio(1.777778);

		mSpringArm->SetActive(false);
		FVector pos = GetActorLocation();
		pos.X -= GetActorForwardVector().X * 400.f;
		pos.Z += 300.f;

		mTempCamera->SetActorLocation(pos);
		mTempCamera->SetActorRotation(FRotator(-20.f, 0.f, 0.f));
	}

	// off.
	else
	{
		mTempCamera->Destroy();
		mSpringArm->SetActive(true);
	}
}

void AWarriorCharacter::SprintJumpStart()
{
	if (mAnimInst->GetCurSkillType() == SKILL_TYPE::SPRINT)
	{
		PrintViewport(1.f, FColor::Red, TEXT("Last"));

		isSprint = true;
		//GetCharacterMovement()->BrakingFrictionFactor = 0.f;
		mSprintDirection = FVector(0.f, 0.f, 1.f);

		LaunchCharacter(mSprintDirection * dist, true, true);
	}
}

void AWarriorCharacter::FinishSprint()
{
	CustomTimeDilation = 1.f;
	isSprint = false;
	mSprintCount = 0;
	GetCharacterMovement()->GravityScale = 1.f;
	GetCharacterMovement()->BrakingFrictionFactor = 2.f;
}
