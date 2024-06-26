// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorCharacter.h"

#include "../Particle/ParticleCascade.h"
//#include "../Basic/WeaponActor.h"
//#include "../Skill/TeleportSkill.h"
#include "PlayerAnimInstance.h"
#include "AGPlayerController.h"
//#include "../Basic/TemporaryfCameraActor.h"
//#include "../Skill/SprintSkil.h"
//#include "../Skill/SlashSkill.h"
#include "../Particle/ParticleNiagara.h"
#include "../Particle/DemonSlash.h"
//#include "../Skill/ContinuousSkill.h"
#include "../Skill/FresnelActor.h"
#include "CharacterStatComponent.h"
#include "../Monster/Monster.h"

AWarriorCharacter::AWarriorCharacter()
{
	//------------------
	// Mesh 세팅.
	//------------------
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>    characterAsset(TEXT("SkeletalMesh'/Game/Assets/Character/Valkyrie/Mesh/SK_Valkyrie.SK_Valkyrie'"));

	if (characterAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(characterAsset.Object);
		mGhostMesh = characterAsset.Object;
	}

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


	static ConstructorHelpers::FClassFinder<UCameraShakeBase>	cameraShake5(TEXT("Blueprint'/Game/Blueprints/CameraShake/CS_PlayerNormalAttack2.CS_PlayerNormalAttack2_C'"));

	if (cameraShake5.Succeeded())
		mNormalAttackShake2 = cameraShake5.Class;


	static ConstructorHelpers::FClassFinder<UCameraShakeBase>	cameraShake4(TEXT("Blueprint'/Game/Blueprints/CameraShake/CS_PlayerSprintAttack.CS_PlayerSprintAttack'"));

	if (cameraShake4.Succeeded())
		mSprintAttackShake = cameraShake4.Class;

	static ConstructorHelpers::FClassFinder<UCameraShakeBase>	cameraShake2(TEXT("Blueprint'/Game/Blueprints/CameraShake/CS_Gontinuous.CS_Gontinuous_C'"));

	if (cameraShake2.Succeeded())
		mContinuousShake = cameraShake2.Class;


	static ConstructorHelpers::FClassFinder<UCameraShakeBase>	cameraShake3(TEXT("Blueprint'/Game/Blueprints/CameraShake/CS_Slash.CS_Slash_C'"));

	if (cameraShake3.Succeeded())
		mSlashShake = cameraShake3.Class;



	//-----------------------------------------
	// HitActor.
	//-----------------------------------------
	mHitActor = CreateDefaultSubobject<AParticleNiagara>(TEXT("HitParticle"));

	AParticleNiagara* particle = Cast<AParticleNiagara>(mHitActor);
	particle->SetParticle(TEXT("NiagaraSystem'/Game/sA_StylizedAttacksPack/FX/NiagaraSystems/NS_BasicHit_2.NS_BasicHit_2'"));
	particle->SetActorScale3D(FVector(3.f, 3.f, 3.f));
	 
	 
	 
	//-----------------------------------------
	// 기타 멤버변수 초기화.
	//-----------------------------------------
	mIsGaugeEnd = false;
	mSprintCount = 0;
	mSprintDirection = FVector(0.f, 0.f, 0.f);
	isSprint = false;

	mContinuousTime = 0.f;

	mFresnelEnable = false;
	mFresnelTime = 0.f;
	mFresnelTimeEnd = 5.f;
	mFresnelCreateTime = 0.f;
	mFresnelCreateTimeEnd = 0.4f;

	mIsSprintLast = false;
}

//void AWarriorCharacter::Skill1End(ASkillActor* SkillActor, UParticleSystemComponent* comp)
//{
//	SkillActor->Destroy();
//}
//
//void AWarriorCharacter::Skill2EndWithNiagara(class ASkillActor* SkillActor, UNiagaraComponent* comp)
//{
//	SkillActor->Destroy();
//}
//
//void AWarriorCharacter::Skill3EndWithNiagara(ASkillActor* SkillActor, UNiagaraComponent* comp)
//{
//	SkillActor->Destroy();
//}
//
//void AWarriorCharacter::Skill4EndWithNiagara(ASkillActor* SkillActor, UNiagaraComponent* comp)
//{
//	SkillActor->Destroy();
//}




void AWarriorCharacter::BeginPlay()
{
	Super::BeginPlay();

	//-----------------------------------------
	// 무기 장착.
	//-----------------------------------------
	FActorSpawnParameters	SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	//mWeapon = GetWorld()->SpawnActor<AWeaponActor>(
	//	AWeaponActor::StaticClass(), SpawnParam);

	//mWeapon->SetSkeletalMesh(TEXT("SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_HeroSword11/SK_Blade_HeroSword11.SK_Blade_HeroSword11'"));


	//mWeapon->AttachToComponent(GetMesh(),
	//	FAttachmentTransformRules::KeepRelativeTransform,
	//	TEXT("thigh_l"));
	//
	//mWeapon->SetActorRelativeLocation(FVector(5.0f, 7.0f, -11.0f));
	//mWeapon->SetActorRelativeRotation(FRotator(10.f, 151.f, 107.f));




	//---------------------------
	// Skill 정보 세팅.
	//---------------------------

	// 1. TELEPORT
	FSkillInfo skillInfo{};
	skillInfo.slotNumber = 0;
	skillInfo.skillType = SKILL_TYPE::TELEPORT;
	skillInfo.minDamage = 100;
	skillInfo.maxDamage = 200;

	/*ATeleportSkill* skillActor = NewObject<ATeleportSkill>(this,
		ATeleportSkill::StaticClass());
	skillInfo.skillActor = Cast<ASkillActor>(skillActor);
	
	skillActor->SetParticle(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Deaths/P_Monster_Death_Large_Fire.P_Monster_Death_Large_Fire'"));
	skillActor->mOnPaticleEnd.AddDynamic(this, &AWarriorCharacter::Skill1End);*/

	mSkillInfoArray.Add(skillInfo);


	// 2. SPRINT
	FSkillInfo skillInfo2{};
	skillInfo2.slotNumber = 1;
	skillInfo2.skillType = SKILL_TYPE::SPRINT;
	skillInfo2.minDamage = 300;
	skillInfo2.maxDamage = 700;

	//ASprintSkil* skillActor2 = NewObject<ASprintSkil>(this,
	//	ASprintSkil::StaticClass());
	//skillInfo2.skillActor = Cast<ASkillActor>(skillActor2);

	//skillActor2->SetNiagara(TEXT("NiagaraSystem'/Game/Hack_And_Slash_FX/VFX_Niagara/Slashes/NS_Demon_Slash.NS_Demon_Slash'"));
	//skillActor2->mOnNiagaraEnd.AddDynamic(this, &AWarriorCharacter::Skill2EndWithNiagara);
	//skillActor2->GetNiagara()->SetActive(true);
	//skillActor2->SetTarget(this);

	mSkillInfoArray.Add(skillInfo2);

	// 3. CONTINUOUS
	FSkillInfo skillInfo3{};
	skillInfo3.slotNumber = 2;
	skillInfo3.skillType = SKILL_TYPE::CONTINUOUS;
	skillInfo3.minDamage = 300;
	skillInfo3.maxDamage = 700;

	/*AContinuousSkill* skillActor3 = NewObject<AContinuousSkill>(this,
		AContinuousSkill::StaticClass());
	skillInfo3.skillActor = Cast<ASkillActor>(skillActor3);

	skillActor3->SetNiagara(TEXT("NiagaraSystem'/Game/StylizedVFX-Atacks/Particles/NS_SlashStrike.NS_SlashStrike'"));
	skillActor3->mOnNiagaraEnd.AddDynamic(this, &AWarriorCharacter::Skill3EndWithNiagara);
	mSkillInfoArray.Add(skillInfo3);*/

	// 4. SLASH
	FSkillInfo skillInfo4{};
	skillInfo4.slotNumber = 3;
	skillInfo4.skillType = SKILL_TYPE::SLASH;
	skillInfo4.minDamage = 300;
	skillInfo4.maxDamage = 700;

	/*ASlashSkill* skillActor4 = NewObject<ASlashSkill>(this,
		ASlashSkill::StaticClass());
	skillInfo4.skillActor = Cast<ASkillActor>(skillActor4);

	skillActor4->SetNiagara(TEXT("NiagaraSystem'/Game/StylizedVFX-Atacks/Particles/NS_SwordsAttack.NS_SwordsAttack'"));
	skillActor4->mOnNiagaraEnd.AddDynamic(this, &AWarriorCharacter::Skill4EndWithNiagara);
	skillActor4->GetNiagara()->SetActive(true);*/
	

	mSkillInfoArray.Add(skillInfo4);

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
				//PrintViewport(10.f, FColor::Red, TEXT("FINISH"));
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
			mFresnelEnable = true;
			GetCharacterMovement()->BrakingFrictionFactor = 0.f;

			// 목표 지점으로 회전.
			FRotator targetRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),
				GetActorLocation() + mSprintDirection * dist);

			FRotator rotator = GetActorRotation();

			rotator = FMath::RInterpTo(rotator, targetRot, DeltaTime, 1000.f);

			SetActorRotation(rotator);
			

			// 이동.
			LaunchCharacter(mSprintDirection * dist, true, true);
			GetWorldTimerManager().SetTimer(timerHandle, this, &AWarriorCharacter::NextSprint, 0.4f, false);
		}

		if (mFresnelEnable)
			SpawnFresnel();
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
			GetWorld()->GetFirstPlayerController()->ClientStopCameraShake(mContinuousShake);
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
	FVector startPosition = GetActorLocation() + GetActorForwardVector() * 10.f;

	mInfo.attackDistance = 100.0f;

	FVector endPosition = startPosition + GetActorForwardVector() * mInfo.attackDistance;

	FCollisionQueryParams	param(NAME_None, false, this);

	TArray<FHitResult>	collisionResult;
	bool IsCollision = false;

	float capsuleRadius = 0.f;
	float capsuletHeight = 0.f;

	switch (mAnimInst->GetPlayerMotion())
	{
	case PLAYER_MOTION::IDLE:
		break;

	case PLAYER_MOTION::MOVE:
		break;

	case PLAYER_MOTION::JUMP:
		break;

	case PLAYER_MOTION::NORMAL_ATTACK:
		if (mAnimInst->GetNormalAttackIndex() == 0)
		{
			startPosition += GetActorForwardVector() * 20.f;
			capsuleRadius = mInfo.attackDistance * 2.f;
			capsuletHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		}
		else
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(mNormalAttackShake);

			capsuleRadius = mInfo.attackDistance;
			capsuletHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		}
		break;

	case PLAYER_MOTION::SKILL:
		if (mAnimInst->GetCurSkillType() == SKILL_TYPE::SPRINT)
		{
			capsuleRadius = mInfo.attackDistance;
			capsuletHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2.f;
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(mSprintAttackShake);

			if (mIsSprintLast)
			{
				GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(mNormalAttackShake2);

				mIsSprintLast = false;
				startPosition = GetActorLocation() - GetActorForwardVector() * 50.f;
				endPosition = startPosition + GetActorForwardVector() * 200.f;
				capsuleRadius = 280.0f;
				capsuletHeight = 200.0f;
			}
		}

		else if (mAnimInst->GetCurSkillType() == SKILL_TYPE::CONTINUOUS)
		{
			startPosition = GetActorLocation() + GetActorForwardVector() * 10.f;
			endPosition = startPosition + GetActorForwardVector() * 100.f;
			capsuleRadius = 100.0f;
			capsuletHeight = 200.0f;
		}

		else if (mAnimInst->GetCurSkillType() == SKILL_TYPE::SLASH)
		{
			startPosition = GetActorLocation() + GetActorForwardVector() * 10.f;
			endPosition = startPosition + GetActorForwardVector() * 300.f;
			capsuleRadius = 400.0f;
			capsuletHeight = 300.0f;
		}
		break;
	}

	


	IsCollision = GetWorld()->SweepMultiByChannel(
		collisionResult, startPosition,
		endPosition, FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeCapsule(capsuleRadius, capsuletHeight),
		param);


#if ENABLE_DRAW_DEBUG
	
	//FColor	DrawColor = IsCollision ? FColor::Red : FColor::Green;

	//DrawDebugCapsule(GetWorld(), (startPosition + endPosition) / 2.f,
	//	capsuletHeight / 2.f,
	//	capsuleRadius,
	//	FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(),
	//	DrawColor, false, 0.5f);

#endif


	if (IsCollision)
	{
		int32	Count = collisionResult.Num();

		for (int32 i = 0; i < Count; ++i)
		{
			if (IsValid(Cast<ACharacter>(collisionResult[i].GetActor())))
			{
				FActorSpawnParameters	SpawnParam;
				SpawnParam.SpawnCollisionHandlingOverride =
					ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				// 나이아가라 렌더링.
				AParticleNiagara* Particle =
					GetWorld()->SpawnActor<AParticleNiagara>(
						collisionResult[i].ImpactPoint,
						collisionResult[i].ImpactNormal.Rotation(),
						SpawnParam);

				Particle->SetParticle(TEXT("NiagaraSystem'/Game/sA_StylizedAttacksPack/FX/NiagaraSystems/NS_BasicHit_2.NS_BasicHit_2'"));

				// 데미지 계산.
				collisionResult[i].GetActor()->TakeDamage(
					(float)mStat->GetAttack(),
					FDamageEvent(),
					GetController(),
					this);
			}
		}
	}
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
	UseSkill(skillType);
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

void AWarriorCharacter::Skill4()
{
	SKILL_TYPE	skillType = SKILL_TYPE::SKILL_TYPE_END;
	int32	count = mSkillInfoArray.Num();

	for (int32 i = 0; i < count; ++i)
	{
		if (mSkillInfoArray[i].slotNumber == 3)
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
			//SpawnSkill(_skillType, i);
			ApplySkill(_skillType);
			mStat->SetMp(mStat->GetMp() - 50.f);
			break;
		}
	}
}

void AWarriorCharacter::SpawnSkill(SKILL_TYPE _skillType, int32 _skillInfoArrayIndex)
{
	FActorSpawnParameters	SpawnParam;
	//SpawnParam.Template = mSkillInfoArray[_skillInfoArrayIndex].skillActor;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	float rand = (float)FMath::FRandRange((float)30.f, (float)50.f);
	
	FVector pos = GetActorLocation();

	switch (_skillType)
	{
	case SKILL_TYPE::SPRINT:
	{
		/*ASprintSkil* skill =
			GetWorld()->SpawnActor<ASprintSkil>(
				pos,
				GetActorRotation(),
				SpawnParam);*/

		StartSlashCameraShake();
		SetIsSprintLast(true);
		NormalAttackCheck();
	}
	break;

	case SKILL_TYPE::CONTINUOUS:
	{
		FRotator targetRot = UKismetMathLibrary::FindLookAtRotation(pos,
			pos + GetActorForwardVector() * 100.f);

		/*AContinuousSkill* Skill =
			GetWorld()->SpawnActor<AContinuousSkill>(
				pos,
				GetActorRotation(),
				SpawnParam);*/

		NormalAttackCheck();
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(mNormalAttackShake);
	}

	case SKILL_TYPE::SLASH:
	{

		pos += (GetActorForwardVector() * 100.f);
		pos.Z -= (GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2.f);

		/*ASlashSkill* Skill =
			GetWorld()->SpawnActor<ASlashSkill>(
				pos,
				GetActorRotation(),
				SpawnParam);*/
	}
	break;
	}
}

void AWarriorCharacter::ApplySkill(SKILL_TYPE _skillType)
{
	Cast<AAGPlayerController>(GetController())->SetInputModeType(INPUT_MODE_TYPE::GAME_ONLY);

	switch (_skillType)
	{
	case SKILL_TYPE::TELEPORT:
	{
		Cast<AAGPlayerController>(GetController())->SetInputModeType(INPUT_MODE_TYPE::GAME_UI);
	}
	break;

	case SKILL_TYPE::SPRINT:
	{
		GetCharacterMovement()->AirControl = 0.2f;
		GetCharacterMovement()->BrakingFrictionFactor = 0.f;
		CustomTimeDilation = 2.f;
	}
	break;

	case SKILL_TYPE::CONTINUOUS:
	{
		//GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(mContinuousShake);
	}

	case SKILL_TYPE::SLASH:
	{
	}
	break;
	}
}

void AWarriorCharacter::EndSkill(SKILL_TYPE _skillType)
{
	Cast<AAGPlayerController>(GetController())->SetInputModeType(INPUT_MODE_TYPE::GAME_ONLY);

	switch (_skillType)
	{
	case SKILL_TYPE::TELEPORT:
		mIsGaugeEnd = false;
		Cast<AAGPlayerController>(GetController())->SetInputModeType(INPUT_MODE_TYPE::GAME_ONLY);
		break;

	case SKILL_TYPE::SPRINT:
		//GetCharacterMovement()->BrakingFrictionFactor = 2.f;
		//isSprint = false;
		//mSprintCount = 0;
		//CustomTimeDilation = 1.f;
		break;

	case SKILL_TYPE::CONTINUOUS:
		mContinuousTime = 0.f;
		break;

	case SKILL_TYPE::SLASH:
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
		//mWeapon->IsGaugeOn(true);
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
		//mSpringArm->bEnableCameraLag = true;
		//mSpringArm->bEnableCameraRotationLag = true;
		//mSpringArm->CameraLagSpeed = 0.5f;
		GetWorld()->GetFirstPlayerController()->ClientStopCameraShake(mGaugeShake);

		// 1. mSpringArm 비활성화.
		

		// 2. 카메라 액터 스폰.
		FActorSpawnParameters	SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride =	ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


		//mTempCamera = GetWorld()->SpawnActor<ATemporaryfCameraActor>(
		//	ATemporaryfCameraActor::StaticClass(), SpawnParam);

		//GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(mTempCamera, 0.8f);
		////mTempCamera->SetCamera(mCamera);
		//mTempCamera->SetSpringArm(mSpringArm);

		//mTempCamera->SetRatio(mCamera->AspectRatio);
		//mTempCamera->GetCamera()->bConstrainAspectRatio = true;
		//mTempCamera->GetCameraComponent()->SetAspectRatio(1.777778);

		FVector target = Cast<AAGPlayerController>(GetController())->GetPickingPosition();
		// 목표 지점으로 회전.
		FRotator targetRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),
			target);

		FRotator rotator = GetActorRotation();

		mSpringArm->SetActive(false);

		FVector pos = GetActorLocation();
		
		//pos.Z += 200.f;

		target.Z += 300.f;
		target += GetActorForwardVector() * 700.f;
		targetRot = UKismetMathLibrary::FindLookAtRotation(target,
			pos);

		/*mTempCamera->SetActorLocation(target);
		mTempCamera->SetActorRotation(targetRot);*/
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
		//mTempCamera->Destroy();
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
		//mWeapon->IsGaugeOn(false);
		//mWeapon->SetTrailOnOff(true);

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
		mAnimInst->ReplaySprintMontage();
	}

	// 이동 공격.
	else if (mSprintCount < 5)
		mAnimInst->ResumSprintAttackMontage();

	// 스킬 끝.
	else
	{
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
	//if (_value)
	//{

	//	FActorSpawnParameters	SpawnParam;
	//	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	//	mTempCamera = GetWorld()->SpawnActor<ATemporaryfCameraActor>(
	//		ATemporaryfCameraActor::StaticClass(), SpawnParam);

	//	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(mTempCamera, 2.f);
	//	//mTempCamera->SetCamera(mCamera);
	//	mTempCamera->SetSpringArm(mSpringArm);

	//	mTempCamera->SetRatio(mCamera->AspectRatio);
	//	mTempCamera->GetCameraComponent()->SetAspectRatio(1.777778);


	//	mSpringArm->SetActive(false);
	//	FVector pos = GetActorLocation();
	//	pos.X -= GetActorForwardVector().X * 400.f;
	//	pos.Z += 300.f;

	//	mTempCamera->SetActorLocation(pos);
	//	mTempCamera->SetActorRotation(FRotator(-20.f, 0.f, 0.f));
	//}

	//// off.
	//else
	//{
	//	mTempCamera->Destroy();
	//	mSpringArm->SetActive(true);
	//}
}

void AWarriorCharacter::SprintJumpStart()
{
	if (mAnimInst->GetCurSkillType() == SKILL_TYPE::SPRINT)
	{
		isSprint = true;
		mSprintDirection = FVector(0.f, 0.f, 1.f);

		SpawnFresnel();
		mFresnelCreateTimeEnd = 0.07f;

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

	ResetFresnel();
}

void AWarriorCharacter::StartSlashCameraShake()
{
	GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(mSlashShake);
}

void AWarriorCharacter::SpawnFresnel()
{
	mFresnelTime += GetWorld()->DeltaTimeSeconds;
	mFresnelCreateTime += GetWorld()->DeltaTimeSeconds;

	if (mFresnelCreateTime >= mFresnelCreateTimeEnd)
	{
		mFresnelCreateTime -= mFresnelCreateTimeEnd;

		// 잔상 생성
		FActorSpawnParameters	SpawnParam;
		//SpawnParam.Template = mHitActor;
		SpawnParam.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AFresnelActor* Ghost =
			GetWorld()->SpawnActor<AFresnelActor>(
				GetMesh()->GetComponentLocation(),
				GetMesh()->GetComponentRotation(),
				SpawnParam);

		Ghost->SetMesh(mGhostMesh);
		Ghost->CopyPoseFromSkeletalComp(GetMesh());
	}

	if (mFresnelTime >= mFresnelTimeEnd)
	{
		mFresnelTime = 0.f;
		mFresnelEnable = false;
	}
}

void AWarriorCharacter::ResetFresnel()
{
	mFresnelEnable = false;
	mFresnelTime = 0.f;
	mFresnelTimeEnd = 5.f;
	mFresnelCreateTime = 0.f;
	mFresnelCreateTimeEnd = 0.4f;
}
