// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"

#include "PlayerCharacter.h"
#include "../Particle/ParticleCascade.h"

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
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (mPlayerState == PLAYER_MOTION::NORMAL_ATTACK || mPlayerState == PLAYER_MOTION::SKILL)
		return;


	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	
	//------------------------
	// Walk <-> Run <-> Dash ��ȯ.
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


		// 1. Dash ������ ���.
		if (mIsDash)
		{
			// ���� ���� �ٷ� �� ����.
			mSpeedValue = 5.f;

			if (!mIsInitDash)
			{
				mIsInitDash = true;
				playerCharacter->SetDashStsteSpeed();
			}
		}


		// 2. Run ������ ���.
		else if (mIsRun)
		{
			// ó�� Run���� �ٲ���� ��� mSpeedValue�� 1 ~ 3 ���� �ٲ� ��, �����Ͽ� ���� ������.
			if (mIsRunInterp)
			{
				playerCharacter->SetRunStateSpeed();

				// 1 ~ 3���� �ٲ��� ��.
				mRunInterpTime += DeltaSeconds;
				mSpeedValue = FMath::Lerp(1.f, 3.f, mRunInterpTime);


				// mSpeedValue�� 3.0f �̻��� �Ǹ� �� �������ʹ� ������ ���� �ʾƵ� ��.
				if (mSpeedValue >= 3.0f)
				{
					mIsRunInterp = false;
					mRunInterpTime = 0.f;
				}
			}

			// Run �����̰�, mSpeedValue ���� 3.0f �̻��� �Ǿ� ������ �� �ʿ䰡 ���� ���.
			else
			{
				mSpeedValue = movement->Velocity.Size() / (movement->MaxWalkSpeed / 3.f);
			}
		}


		// 3. Walk ������ ���.
		else
		{
			playerCharacter->SetWalkStsteSpeed();
			mSpeedValue = movement->Velocity.Size() / movement->MaxWalkSpeed;	// MaxWalkSpeed: 600
		}

		//PrintViewport(0.5f, FColor::Red, FString::Printf(TEXT("mSpeedValue: %f, walkSpeed: %f"), mSpeedValue, movement->MaxWalkSpeed));





		// �������� ���߸� MOVE -> IDLE ����.
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



		// walk ���¿��� 1�� ������ run ���� �ٲ��.
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
	// Idle ���¿��� ������ �ٲ��� �� ��Ÿ�� ����� ������, 
	// ���彺���̽� mPlayModeValue �� �����ؼ� ���Ŀ��� ������ ���� �����ϵ��� ��.
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
	PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("AnimNotify_JumpEnd")));
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
	// ���� ���� ���� �̾ �ص� ��.
	mIsNormalAttackEnable = true;
}

void UPlayerAnimInstance::AnimNotify_AttackEnd()
{
	// ������ �������� �ٽ� ó������ ������ �� �ֵ��� ���� ���� �ʱ�ȭ.
	mNormalAttackIndex = 0;
	mIsNormalAttackEnable = true;
	mIsNormalAttack = false;

	mPlayerState = PLAYER_MOTION::IDLE;
	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(playerCharacter))
	{
		playerCharacter->SetWeaponTrailOnOff(false);
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
	mPlayerState = PLAYER_MOTION::IDLE;

	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(playerCharacter))
	{
		playerCharacter->SetWeaponTrailOnOff(false);
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
		FVector position = FVector(playerPosition.X+70.f, playerPosition.Y, 60.f);
		AParticleCascade* Particle =
			GetWorld()->SpawnActor<AParticleCascade>(
				position,
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
		FVector playerPosition = playerCharacter->GetActorLocation();
		playerPosition += playerCharacter->GetActorForwardVector() * 200.f;
		playerPosition.Z = 60.f;
		AParticleCascade* Particle =
			GetWorld()->SpawnActor<AParticleCascade>(
				playerPosition,
				playerCharacter->GetActorRotation(),
				SpawnParam);

		Particle->SetParticle(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Monsters/FX_Monster_Deaths/P_Monster_Death_Large_Fire.P_Monster_Death_Large_Fire'"));
	}
}


void UPlayerAnimInstance::ChangePlayMode()
{
	// ���� �����̴� ���̸� ��Ÿ�ִ� ������� �ʰ�, �÷��� ��常 ����.
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
	PrintViewport(0.5f, FColor::Red, FString::Printf(TEXT("JUMP")));
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
	// ���� ���ϴ� ������ ��� ����.
	if (!mIsNormalAttackEnable)
		return;

	// ���� ���� �����ҰŴϱ� �� ���� ���������� ���� ����.
	mIsNormalAttackEnable = false;

	if (!Montage_IsPlaying(mNormalAttackMontageArray[mNormalAttackIndex]))
	{

		// 0.f: ���� ��ġ ����.
		Montage_SetPosition(mNormalAttackMontageArray[mNormalAttackIndex], 0.f);

		// ��Ÿ�� ���.
		Montage_Play(mNormalAttackMontageArray[mNormalAttackIndex]);
		
		// 0, 1, 2, 3 �ݺ�
		mNormalAttackIndex = (mNormalAttackIndex + 1) % mNormalAttackMontageArray.Num();

		// ���� ������.
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
