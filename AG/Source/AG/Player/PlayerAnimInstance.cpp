// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"

#include "PlayerCharacter.h"
#include "../Particle/ParticleCascade.h"
#include "WarriorCharacter.h"
#include "../Particle/ParticleNiagara.h"
#include "../Skill/SprintSkil.h"
#include "../Skill/ContinuousSkill.h"
#include "AGPlayerController.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
	mCurPlayMode = PLAYE_MODE::BATTLE;
	mPlayerState = PLAYER_MOTION::IDLE;

	mPlayModeValue = 2.f;
	mSpeedValue = 0.f;

	mMovingTime = 0.f;
	mIsRun = false;

	mRunInterpTime = 0.f;
	mIsRunInterp = false;

	mIsDash = false;
	mIsInitDash = false;

	mIsAir = false;
	mIsGround = true;
	mIsLandStart = false;

	mIsNormalAttackEnable = true;
	mIsNormalAttack = false;
	mNormalAttackIndex = 0;

	mCurSkillType = SKILL_TYPE::SKILL_TYPE_END;
	mCurSkillPlayingIndex = -1;

	static ConstructorHelpers::FClassFinder<UCameraShakeBase>	cameraShake2(TEXT("Blueprint'/Game/Blueprints/CameraShake/CS_PlayerNormalAttack2.CS_PlayerNormalAttack2_C'"));

	if (cameraShake2.Succeeded())
		mNormalAttackShake2 = cameraShake2.Class;

	static ConstructorHelpers::FClassFinder<UCameraShakeBase>	cameraShake3(TEXT("Blueprint'/Game/Blueprints/CameraShake/CS_Teleport.CS_Teleport_C'"));

	if (cameraShake3.Succeeded())
		mTel = cameraShake3.Class;

	static ConstructorHelpers::FClassFinder<UCameraShakeBase>	cameraShake(TEXT("Blueprint'/Game/Blueprints/CameraShake/CS_PlayerNormalAttack.CS_PlayerNormalAttack_C'"));

	if (cameraShake.Succeeded())
		mNormalCS = cameraShake.Class;

	mSprintCount = 0;
	mContinuousCount = 0;

	mIsEndContinuous = false;

	mHitMontageIndex = 0;
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (mPlayerState == PLAYER_MOTION::NORMAL_ATTACK || mPlayerState == PLAYER_MOTION::SKILL
		|| mPlayerState == PLAYER_MOTION::HIT)
		return;


	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	
	//------------------------
	// Walk <-> Run <-> Dash 전환.
	//------------------------
	if (IsValid(playerCharacter))
	{
		UCharacterMovementComponent* movement = playerCharacter->GetCharacterMovement();

		mIsGround = movement->IsMovingOnGround();
		mIsAir = movement->IsFalling();
		if (mIsAir)
		{
			//PrintViewport(0.5f, FColor::Red, FString::Printf(TEXT("mIsAir")));
			return;
		}


		// 1. Dash 상태일 경우.
		if (mIsDash)
		{
			// 보간 없이 바로 값 변경.
			mSpeedValue = 5.f;

			if (!mIsInitDash)
			{
				mIsInitDash = true;
				playerCharacter->SetDashStsteSpeed();
			}
		}


		// 2. Run 상태일 경우.
		else if (mIsRun)
		{
			// 처음 Run으로 바뀌었을 경우 mSpeedValue가 1 ~ 3 으로 바뀔 때, 보간하여 값을 지정함.
			if (mIsRunInterp)
			{
				playerCharacter->SetRunStateSpeed();

				// 1 ~ 3으로 바뀌어야 함.
				mRunInterpTime += DeltaSeconds;
				mSpeedValue = FMath::Lerp(1.f, 3.f, mRunInterpTime);


				// mSpeedValue가 3.0f 이상이 되면 그 다음부터는 보간을 하지 않아도 됨.
				if (mSpeedValue >= 3.0f)
				{
					mIsRunInterp = false;
					mRunInterpTime = 0.f;
				}
			}

			// Run 상태이고, mSpeedValue 값이 3.0f 이상이 되어 보간을 할 필요가 없을 경우.
			else
			{
				mSpeedValue = movement->Velocity.Size() / (movement->MaxWalkSpeed / 3.f);// MaxWalkSpeed: 1000
			}
		}


		// 3. Walk 상태일 경우.
		else
		{
			playerCharacter->SetWalkStsteSpeed();
			mSpeedValue = movement->Velocity.Size() / movement->MaxWalkSpeed;	// MaxWalkSpeed: 600
		}

		//PrintViewport(0.5f, FColor::Red, FString::Printf(TEXT("mSpeedValue: %f, walkSpeed: %f"), mSpeedValue, movement->MaxWalkSpeed));





		// 움직임을 멈추면 MOVE -> IDLE 변경.
		if (mSpeedValue <= 0.f)
		{
			mIsRun = false;
			mIsRunInterp = false;
			mMovingTime = 0.f;
			mRunInterpTime = 0.f;
			mPlayerState = PLAYER_MOTION::IDLE;
		}
		else
			mPlayerState = PLAYER_MOTION::MOVE;



		// walk 상태에서 1초 지나면 run 으로 바뀐다.
		if (mSpeedValue >= 0.9f && !mIsRun)
		{
			mMovingTime += DeltaSeconds;

			if (mMovingTime >= 1.f)
			{
				PrintViewport(0.5f, FColor::Red, FString::Printf(TEXT("change")));
				mIsRun = true;
				mIsRunInterp = true;
				mMovingTime = 0.f;
			}
		}
	}



	//------------------------
	// Idle 상태에서 동작을 바꿨을 때 몽타주 재생이 끝나면, 
	// 블렌드스페이스 mPlayModeValue 값 세팅해서 이후에도 동일한 상태 유지하도록 함.
	//------------------------
	switch (mCurPlayMode)
	{
	case PLAYE_MODE::NO_BATTLE:
		if (!Montage_IsPlaying(mIdleModeMontageArray[1]))
		{
			mPlayModeValue = 0.f;
		}
		break;

	case PLAYE_MODE::BATTLE:
		if (!Montage_IsPlaying(mIdleModeMontageArray[0]))
		{
			mPlayModeValue = 1.f;
		}
		break;
	}

}

void UPlayerAnimInstance::AnimNotify_JumpEnd()
{
	mPlayerState = PLAYER_MOTION::IDLE;
}

void UPlayerAnimInstance::AnimNotify_EquipSword()
{
	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(playerCharacter))
	{
		playerCharacter->EquipWeaponToHand();
	}
}

void UPlayerAnimInstance::AnimNotify_AttackCheck()
{
	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(playerCharacter))
	{
		playerCharacter->NormalAttackCheck();
	}
}

void UPlayerAnimInstance::AnimNotify_AttackEnable()
{
	// 이제 다음 공격 이어서 해도 됨.
	mIsNormalAttackEnable = true;
}

void UPlayerAnimInstance::AnimNotify_AttackEnd()
{
	// 공격이 끝났으니 다시 처음부터 공격할 수 있도록 공격 상태 초기화.
	mNormalAttackIndex = 0;
	mIsNormalAttackEnable = true;
	mIsNormalAttack = false;

	mPlayerState = PLAYER_MOTION::IDLE;
	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(playerCharacter))
	{
		playerCharacter->SetWeaponTrailOnOff(false);
		playerCharacter->SetGameModeGameOnly();
	}
}

void UPlayerAnimInstance::AnimNotify_Delay()
{

	Montage_SetPlayRate(mNormalAttackMontageArray[3], 0.3f);
}

void UPlayerAnimInstance::AnimNotify_Reset()
{
	Montage_SetPlayRate(mNormalAttackMontageArray[3], 1.2f);


}

void UPlayerAnimInstance::AnimNotify_SkillEnd()
{
	
	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(playerCharacter))
	{
		playerCharacter->SetWeaponTrailOnOff(false);
		
	}

	if (mCurSkillType == SKILL_TYPE::TELEPORT)
	{
		mPlayerState = PLAYER_MOTION::IDLE;
		playerCharacter->EndSkill(mCurSkillType);
		mCurSkillType = SKILL_TYPE::SKILL_TYPE_END;
	}

	else if (mCurSkillType == SKILL_TYPE::SPRINT)
	{
		mPlayerState = PLAYER_MOTION::SKILL;
		
		++mSprintCount;
		Montage_SetPlayRate(mSkillMontageArray[mCurSkillPlayingIndex].Montage, 1.f);

		// 스킬이 아예 끝났다> IDLE로 돌아감.
		if (mSprintCount == 5)
		{
			Cast<AWarriorCharacter>(playerCharacter)->FinishSprint();
			
			mSprintCount = 0;
			mPlayerState = PLAYER_MOTION::IDLE;
			mCurSkillType = SKILL_TYPE::SKILL_TYPE_END;
			playerCharacter->EndSkill(mCurSkillType);
		}
		else if (mSprintCount < 4)
		{
			// 플레이어가 다시 이동할 수 있게 함.
			Montage_SetPosition(mSkillMontageArray[mCurSkillPlayingIndex].Montage, 026.f);

			playerCharacter->SetISEnableSprint(false);

			Montage_Play(mSkillMontageArray[mCurSkillPlayingIndex].Montage);
		}
		// 마지막 점프 공격.
		else if (mSprintCount == 4)
		{
			Montage_SetPosition(mSkillMontageArray[mCurSkillPlayingIndex].Montage, 1.96);
			Montage_SetPlayRate(mSkillMontageArray[mCurSkillPlayingIndex].Montage, 0.5f);
			
			Cast<AWarriorCharacter>(playerCharacter)->SprintJumpStart();
		}
	}

	else if (mCurSkillType == SKILL_TYPE::CONTINUOUS)
	{
		if (!mIsEndContinuous)
		{
			Montage_SetPosition(mSkillMontageArray[mCurSkillPlayingIndex].Montage, 0.0f);
			Montage_Play(mSkillMontageArray[mCurSkillPlayingIndex].Montage);
			
		}
		else
		{
			mContinuousCount = 0;
			mIsEndContinuous = false;
			mPlayerState = PLAYER_MOTION::IDLE;
			mCurSkillType = SKILL_TYPE::SKILL_TYPE_END;
			playerCharacter->EndSkill(mCurSkillType);
		}
	}

	else if (mCurSkillType == SKILL_TYPE::SLASH)
	{
		mPlayerState = PLAYER_MOTION::IDLE;
		mCurSkillType = SKILL_TYPE::SKILL_TYPE_END;
		playerCharacter->EndSkill(mCurSkillType);
	}
}

void UPlayerAnimInstance::AnimNotify_StartGauge()
{
	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(playerCharacter))
	{
		float gauge = playerCharacter->GetTeleportGaueTime();

		if (gauge > 0.f && mCurSkillPlayingIndex != -1)
		{
			Montage_Pause(mSkillMontageArray[mCurSkillPlayingIndex].Montage);
		}
	}
}

void UPlayerAnimInstance::AnimNotify_LastNormalAttack()
{
	GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(mNormalAttackShake2);

	FActorSpawnParameters	SpawnParam;
	//SpawnParam.Template = mHitActor;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(playerCharacter))
	{
		FVector playerPosition = playerCharacter->GetActorLocation();
		playerPosition.Z -= (playerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() / 2.f);

		AParticleCascade* Particle =
			GetWorld()->SpawnActor<AParticleCascade>(
				playerPosition,
				playerCharacter->GetActorRotation(),
				SpawnParam);

		Particle->SetParticle(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Mobile/Fire/combat/P_Fire_AOE_Blast_mobile.P_Fire_AOE_Blast_mobile'"));
	}
}

void UPlayerAnimInstance::AnimNotify_Teleport()
{
	GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(mTel);
}

void UPlayerAnimInstance::AnimNotify_TeleportEff()
{
	FActorSpawnParameters	SpawnParam;
	//SpawnParam.Template = mHitActor;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(playerCharacter))
	{
		playerCharacter->SpawnSkill(mCurSkillType, mCurSkillPlayingIndex);

		//FVector playerPosition = playerCharacter->GetActorLocation();
		//playerPosition += playerCharacter->GetActorForwardVector() * 200.f;
		//playerPosition.Z = 30.f;
		//AParticleCascade* Particle =
		//	GetWorld()->SpawnActor<AParticleCascade>(
		//		playerPosition,
		//		playerCharacter->GetActorRotation(),
		//		SpawnParam);

		//Particle->SetParticle(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Deaths/P_Monster_Death_Large_Fire.P_Monster_Death_Large_Fire'"));
	
		Cast<AWarriorCharacter>(playerCharacter)->StartSlashCameraShake();
	}
}

void UPlayerAnimInstance::AnimNotify_ResetSpeed()
{
	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(playerCharacter))
	{
		playerCharacter->GetCharacterMovement()->GravityScale = 5.f;
		playerCharacter->GetCharacterMovement()->StopMovementImmediately();
		Montage_SetPlayRate(mSkillMontageArray[mCurSkillPlayingIndex].Montage, 1.f);
	}

}

void UPlayerAnimInstance::AnimNotify_Continuous()
{
	mContinuousCount++;

	if (!mIsEndContinuous)
	{
		Montage_SetPosition(mSkillMontageArray[mCurSkillPlayingIndex].Montage, 0.47);
	}
}

void UPlayerAnimInstance::AnimNotify_EffectSpawn()
{
	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(playerCharacter))
	{
		if (mCurSkillType == SKILL_TYPE::SPRINT)
		{
			playerCharacter->SpawnSkill(SKILL_TYPE::SPRINT, 1);
			Cast<AWarriorCharacter>(playerCharacter)->StartSlashCameraShake();
			Cast<AWarriorCharacter>(playerCharacter)->SetIsSprintLast(true);
			Cast<AWarriorCharacter>(playerCharacter)->NormalAttackCheck();
		}

		else if (mCurSkillType == SKILL_TYPE::CONTINUOUS)
		{
			Cast<AWarriorCharacter>(playerCharacter)->NormalAttackCheck();
			playerCharacter->SpawnSkill(SKILL_TYPE::CONTINUOUS, mCurSkillPlayingIndex);
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(mNormalCS);
		}

		else if (mCurSkillType == SKILL_TYPE::SLASH)
		{
			playerCharacter->SpawnSkill(SKILL_TYPE::SLASH, mCurSkillPlayingIndex);
		}
	}
}

void UPlayerAnimInstance::AnimNotify_SlowStop()
{
	Montage_SetPlayRate(mSkillMontageArray[mCurSkillPlayingIndex].Montage, 0.8f);
}

void UPlayerAnimInstance::AnimNotify_SlashCS()
{
	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(playerCharacter))
	{
		Cast<AWarriorCharacter>(playerCharacter)->StartSlashCameraShake();
	}
}


void UPlayerAnimInstance::AnimNotify_TarilOn()
{
	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(playerCharacter))
	{
		playerCharacter->SetWeaponTrailOnOff(true);
	}
}

void UPlayerAnimInstance::AnimNotify_TarilOff()
{
	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(playerCharacter))
	{
		playerCharacter->SetWeaponTrailOnOff(false);
	}
}

void UPlayerAnimInstance::AnimNotify_HitEnd()
{
	mPlayerState = PLAYER_MOTION::IDLE;
}




void UPlayerAnimInstance::ChangePlayMode()
{
	// 현재 움직이는 중이면 몽타주는 재생하지 않고, 플레이 모드만 변경.
	if (mPlayerState != PLAYER_MOTION::IDLE)
	{
		switch (mCurPlayMode)
		{
		case PLAYE_MODE::NO_BATTLE:
			mCurPlayMode = PLAYE_MODE::BATTLE;
			mPlayModeValue = 1.f;
			break;

		case PLAYE_MODE::BATTLE:
			mCurPlayMode = PLAYE_MODE::NO_BATTLE;
			mPlayModeValue = 0.f;
			break;

		}
		return;
	}


	switch (mCurPlayMode)
	{
	case PLAYE_MODE::NO_BATTLE:	

		mCurPlayMode = PLAYE_MODE::BATTLE;

		if (!Montage_IsPlaying(mIdleModeMontageArray[0]))
		{
			Montage_SetPosition(mIdleModeMontageArray[0], 0.f);
			Montage_Play(mIdleModeMontageArray[0]);
		}
		break;


	case PLAYE_MODE::BATTLE:	

		mCurPlayMode = PLAYE_MODE::NO_BATTLE;

		if (!Montage_IsPlaying(mIdleModeMontageArray[1]))
		{
			Montage_SetPosition(mIdleModeMontageArray[1], 0.f);
			Montage_Play(mIdleModeMontageArray[1]);
		}
		break;
	}


}

void UPlayerAnimInstance::Evade(DIRECTION direction)
{
	int index = (uint8)direction;

	if (!Montage_IsPlaying(mEvadeMontage[index]))
	{
		Montage_SetPosition(mEvadeMontage[index], 0.f);
		Montage_Play(mEvadeMontage[index]);
	}
}

void UPlayerAnimInstance::JumpStart()
{
	mPlayerState = PLAYER_MOTION::JUMP;
	mIsLandStart = false;
}

void UPlayerAnimInstance::Dash()
{
	mIsDash = true;
	mIsInitDash = false;
}

void UPlayerAnimInstance::FinishDash()
{
	mIsDash = false;
	mIsInitDash = false;
}

void UPlayerAnimInstance::EquipWeapon()
{
	if (!Montage_IsPlaying(mEquipWeaponMontage))
	{
		Montage_SetPosition(mEquipWeaponMontage, 0.f);
		Montage_Play(mEquipWeaponMontage);
	}
}

void UPlayerAnimInstance::NormalAttack()
{
	// 공격 못하는 상태일 경우 리턴.
	if (!mIsNormalAttackEnable)
	{
		return;
	}

	// 이제 공격 시작할거니까 이 공격 끝날때까지 공격 못함.
	mIsNormalAttackEnable = false;

	if (!Montage_IsPlaying(mNormalAttackMontageArray[mNormalAttackIndex]))
	{

		// 0.f: 시작 위치 지정.
		Montage_SetPosition(mNormalAttackMontageArray[mNormalAttackIndex], 0.f);

		// 몽타주 재생.
		Montage_Play(mNormalAttackMontageArray[mNormalAttackIndex]);
		
		// 0, 1, 2, 3 반복
		mNormalAttackIndex = (mNormalAttackIndex + 1) % mNormalAttackMontageArray.Num();

		// 현재 공격중.
		mIsNormalAttack = true;

		mPlayerState = PLAYER_MOTION::NORMAL_ATTACK;

	}
	else
		mIsNormalAttackEnable = true;
}

void UPlayerAnimInstance::UseSkill(SKILL_TYPE _skillType)
{
	int32	count = mSkillMontageArray.Num();

	for (int32 i = 0; i < count; ++i)
	{
		if (mSkillMontageArray[i].skillType == _skillType)
		{
			mCurSkillType = _skillType;
			mPlayerState = PLAYER_MOTION::SKILL;


			if (!Montage_IsPlaying(mSkillMontageArray[i].Montage))
			{
				if (mCurSkillType == SKILL_TYPE::SLASH)
				{
					Montage_SetPlayRate(mSkillMontageArray[i].Montage, 0.3f);
				}
				else
					Montage_SetPlayRate(mSkillMontageArray[i].Montage, 1.f);
				Montage_SetPosition(mSkillMontageArray[i].Montage, 0.f);
				Montage_Play(mSkillMontageArray[i].Montage);

				mCurSkillPlayingIndex = i;
			}

			break;
		}
	}
}

void UPlayerAnimInstance::GaugeEnd()
{
	Montage_SetPosition(mSkillMontageArray[mCurSkillPlayingIndex].Montage, 0.84f);
}

void UPlayerAnimInstance::RestartSkill()
{
	Montage_SetPlayRate(mSkillMontageArray[mCurSkillPlayingIndex].Montage, 0.3f);
	Montage_Resume(mSkillMontageArray[mCurSkillPlayingIndex].Montage);
}

void UPlayerAnimInstance::ResumSprintAttackMontage()
{
	if (mPlayerState != PLAYER_MOTION::SKILL || mCurSkillType != SKILL_TYPE::SPRINT)
		return;


	if (mSprintCount < 5)
	{
		//PrintViewport(1.f, FColor::Green, TEXT("ResumSSprintAttackMontage()"));
		Montage_SetPosition(mSkillMontageArray[mCurSkillPlayingIndex].Montage, 1.13f);
	}
	else if (mSprintCount == 5)
	{
		//PrintViewport(1.f, FColor::Green, TEXT("ResumSprintAttackMontage()"));
	}
	Montage_SetPlayRate(mSkillMontageArray[mCurSkillPlayingIndex].Montage, 1.3f);

	//Montage_Resume(mSkillMontageArray[mCurSkillPlayingIndex].Montage);
}

void UPlayerAnimInstance::ReplaySprintMontage()
{
	//Montage_Play(mSkillMontageArray[mCurSkillPlayingIndex].Montage);
	PrintViewport(10.f, FColor::Red, FString::Printf(TEXT("ReplaySprintMontage")));
	Montage_SetPlayRate(mSkillMontageArray[mCurSkillPlayingIndex].Montage, 1.5f);

	Montage_Resume(mSkillMontageArray[mCurSkillPlayingIndex].Montage);
	
}

void UPlayerAnimInstance::StopContinuousSkill()
{
	mIsEndContinuous = true;
	mPlayerState = PLAYER_MOTION::IDLE;
	mCurSkillType = SKILL_TYPE::SKILL_TYPE_END;
}

void UPlayerAnimInstance::SetHitDirection(FString _value)
{
	mPlayerState = PLAYER_MOTION::HIT;
	if (_value == TEXT("Front"))
	{
		mHitMontageIndex = 0;
	}
	else if (_value == TEXT("Back"))
	{
		mHitMontageIndex = 1;
	}
	else if (_value == TEXT("Left"))
	{
		mHitMontageIndex = 2;
	}
	else if (_value == TEXT("Right"))
	{
		mHitMontageIndex = 3;
	}

	Hit();
}

void UPlayerAnimInstance::Hit()
{
	if (!Montage_IsPlaying(mHitMontageArray[mHitMontageIndex]))
	{
		Montage_SetPosition(mHitMontageArray[mHitMontageIndex], 0.f);
		Montage_Play(mHitMontageArray[mHitMontageIndex]);
	}
}
