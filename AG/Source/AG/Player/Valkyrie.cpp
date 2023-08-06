// Fill out your copyright notice in the Description page of Project Settings.


#include "Valkyrie.h"
#include "ValkyrieAnimInstance.h"
#include "Sword.h"
#include "MotionWarpingComponent.h"
#include "RootMotionModifier.h"
#include "../Particle/ParticleCascade.h"
#include "../Particle/ParticleNiagara.h"
#include "../Particle/ValkyrieSlash.h"
#include "../Particle/ValkyrieLightning.h"

AValkyrie::AValkyrie()
{
	//------------------
	// Mesh ¼¼ÆÃ.
	//------------------
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>    characterAsset(TEXT("SkeletalMesh'/Game/Assets/Character/Valkyrie/Mesh/SK_Valkyrie.SK_Valkyrie'"));
	if (characterAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(characterAsset.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0.0, 0.0, -90.0));
	GetMesh()->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));


	static ConstructorHelpers::FClassFinder <UValkyrieAnimInstance>    animInst(TEXT("AnimBlueprint'/Game/Blueprints/Valkyrie/Animations/ABP_Valkyrie.ABP_Valkyrie_C'"));
	if (animInst.Succeeded())
	{
		GetMesh()->SetAnimClass(animInst.Class);
	}


	UAnimMontage* montage;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> equipMontage(TEXT("AnimMontage'/Game/Blueprints/Valkyrie/Animations/Montages/AM_Valkyrie_Equip.AM_Valkyrie_Equip'"));
	if (equipMontage.Succeeded())
	{
		montage = equipMontage.Object;
	}
	mMontages.Add(FName("Equip"), montage);

	UAnimMontage* montage2;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> attackMontage(TEXT("AnimMontage'/Game/Blueprints/Valkyrie/Animations/Montages/AM_Valkyrie_Attack.AM_Valkyrie_Attack'"));
	if (attackMontage.Succeeded())
	{
		montage2 = attackMontage.Object;
	}
	mMontages.Add(FName("Attack"), montage2);

	UAnimMontage* montage3;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> sprintMontage(TEXT("AnimMontage'/Game/Blueprints/Valkyrie/Animations/Montages/AM_Valkyrie_Sprint.AM_Valkyrie_Sprint'"));
	if (sprintMontage.Succeeded())
	{
		montage3 = sprintMontage.Object;
	}
	mMontages.Add(FName("Sprint"), montage3);

	UAnimMontage* montage4;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ribbonMontage(TEXT("AnimMontage'/Game/Blueprints/Valkyrie/Animations/Montages/AM_Valkyrie_Ribbon.AM_Valkyrie_Ribbon'"));
	if (ribbonMontage.Succeeded())
	{
		montage4 = ribbonMontage.Object;
	}
	mMontages.Add(FName("Ribbon"), montage4);

	UAnimMontage* montage5;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> slashMontage(TEXT("AnimMontage'/Game/Blueprints/Valkyrie/Animations/Montages/AM_Valkyrie_Slash.AM_Valkyrie_Slash'"));
	if (slashMontage.Succeeded())
	{
		montage5 = slashMontage.Object;
	}
	mMontages.Add(FName("Slash"), montage5);


	mAttackMaxIndex = 4;
	NormalAttackEnd();


	mMotionWarpComp = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpComp"));


	mCameraOne = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraOne"));
	mCameraOne->SetupAttachment(mSpringArmComp);
	mCameraOne->SetActive(false);


	tempLocation = FVector(0.f);

	JumpMaxCount = 2;
	GetCharacterMovement()->JumpZVelocity = 500.f;
	mJumpAttackIndex = -1;
}

void AValkyrie::BeginPlay()
{
	Super::BeginPlay();

	mAnimInst = Cast<UValkyrieAnimInstance>(GetMesh()->GetAnimInstance());

	FActorSpawnParameters	SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	mWeapon = GetWorld()->SpawnActor<ASword>(ASword::StaticClass(), SpawnParam);
	mWeapon->SetSkeletalMesh(TEXT("SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_HeroSword11/SK_Blade_HeroSword11.SK_Blade_HeroSword11'"));
	mWeapon->Equip(GetMesh(), TEXT("UnEquipSword"), this, this);
	mWeapon->SetCollisionOnOff(false);

	SetAnimDelegate();
}

void AValkyrie::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AValkyrie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (mActionState != EActionState::EAS_JumpAttack)
	//	mIsJumpAttack = false;
	//if (mActionState == EActionState::EAS_JumpAttack)
	//	PrintViewport(1.f, FColor::Red, TEXT("Ffff"));
}

void AValkyrie::PlayMontage(FName _montageName, FName _sectionName)
{
	UAnimMontage* montage = *mMontages.Find(_montageName);

	mAnimInst->Montage_Play(montage);
	if (_montageName == FName("Equip") || _montageName == FName("Attack") || _montageName == FName("JumpAttack"))
	{
		mAnimInst->Montage_JumpToSection(_sectionName, montage);
	}
}

void AValkyrie::EquipWeaponKey()
{
	FName sectionName = FName("");

	if (mCharacterState == ECharacterState::ECS_Unequipped)
	{
		mCharacterState = ECharacterState::ECS_EquippedWeapon;
		sectionName = FName("Equip");
		mDirection = 2.f;

		if (mWeapon)
		{
			mWeapon->Equip(GetMesh(), TEXT("EquipSword"), this, this);
		}
	}
	else if (mCharacterState == ECharacterState::ECS_EquippedWeapon)
	{
		mCharacterState = ECharacterState::ECS_Unequipped;
		sectionName = FName("UnEquip");
		mDirection = 0.f;
	}

	PlayMontage(FName("Equip"), sectionName);
}

void AValkyrie::NormalAttackKey()
{
	if (mCharacterState == ECharacterState::ECS_Unequipped)
		return;

	mWeapon->SetCollisionOnOff(true);

	if (mIsJumpAttack)
	{
		
		mActionState = EActionState::EAS_JumpAttack;
		
		if (mAnimInst->GetIsJumpAttackEnd())
		{
			if (mJumpAttackIndex == 2)
				mJumpAttackIndex = 0;
			else
				mJumpAttackIndex = FMath::Clamp<int32>(mJumpAttackIndex + 1, 0, 2);
			LaunchCharacter(FVector(0.f, 0.f, 200.f), true, true);
			PrintViewport(0.5f, FColor::Blue, TEXT("Jump!"));
		}
		PrintViewport(0.5f, FColor::Red, FString::Printf(TEXT("JumpAttack: %d"), mJumpAttackIndex));
		return;
	}

	if (mIsAttacking)
	{
		if (mIsCanNextAttack)
			mIsAttackInputOn = true;
	}
	else
	{
		mIsAttacking = true;
		NormalAttackStart();
		if (mActionState != EActionState::EAS_JumpAttack)
			PlayMontage(FName("Attack"), FName(*FString::Printf(TEXT("Attack%d"), mCurrentAttackIndex)));
	}


}

void AValkyrie::Skill1Key()
{
	mSkillState = ESkillState::ESS_Sprint;
	mWeapon->SetCollisionOnOff(false);
	PlayMontage(FName("Sprint"));
	FVector targetLocation = GetActorLocation();
	tempLocation = targetLocation;
	targetLocation += GetActorForwardVector() * 1000.f;
	mMotionWarpComp->AddOrUpdateWarpTargetFromLocation(FName("SprintTarget2"), targetLocation);
}

void AValkyrie::Skill2Key()
{
	mSkillState = ESkillState::ESS_Ribbon;
	PlayMontage(FName("Ribbon"));
	mWeapon->SetCollisionOnOff(false);

	FVector location = GetActorLocation();
	FRotator rotation = GetActorRotation();

	{
		FVector dir = GetActorForwardVector() + GetActorRightVector();	// 1
		FVector targetLocation = GetActorLocation() + dir * 300.f;
		FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),
			targetLocation);

		location = targetLocation;
		rotation = targetRotation;

		mMotionWarpComp->AddOrUpdateWarpTargetFromLocationAndRotation(FName("Ribbon1"), targetLocation, targetRotation);
	}
	
	{
		FVector dir = UKismetMathLibrary::GetRightVector(rotation);
		FVector targetLocation = location + dir * 300.f;
		FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(location,
			targetLocation);

		location = targetLocation;
		rotation = targetRotation;

		mMotionWarpComp->AddOrUpdateWarpTargetFromLocationAndRotation(FName("Ribbon2"), targetLocation, targetRotation);
	}

	{
		FVector right = UKismetMathLibrary::GetRightVector(rotation);
		FVector targetLocation = location + right * 300.f;
		FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(location,
			targetLocation);

		location = targetLocation;
		rotation = targetRotation;

		mMotionWarpComp->AddOrUpdateWarpTargetFromLocationAndRotation(FName("Ribbon3"), targetLocation, targetRotation);
	}

	{
		FVector dir = UKismetMathLibrary::GetRightVector(rotation);
		FVector targetLocation = location + dir * 300.f;
		
		FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(location,
			targetLocation);

		location = targetLocation;
		rotation = targetRotation;

		mMotionWarpComp->AddOrUpdateWarpTargetFromLocationAndRotation(FName("Ribbon4"), targetLocation, targetRotation);
	}
}

void AValkyrie::Skill3Key()
{
	mSkillState = ESkillState::ESS_Slash;
	PlayMontage(FName("Slash"));
	mWeapon->SetCollisionOnOff(false);
}

void AValkyrie::NormalAttackStart()
{
	if (mCurrentAttackIndex == 4)
		mCurrentAttackIndex = 1;	
	mIsCanNextAttack = true;
	mIsAttackInputOn = false;
	//if (FMath::IsWithinInclusive<int32>(mCurrentAttackIndex, 0, mAttackMaxIndex))
	mCurrentAttackIndex = FMath::Clamp<int32>(mCurrentAttackIndex + 1, 1, mAttackMaxIndex);
	
	if (mIsJumpAttack)
		mActionState = EActionState::EAS_JumpAttack;
	else
		mActionState = EActionState::EAS_Attack;
}

void AValkyrie::NormalAttackEnd()
{
	mIsAttackInputOn = false;
	mIsCanNextAttack = false;
	mCurrentAttackIndex = 0;
	mActionState = EActionState::EAS_Idle;
}

void AValkyrie::SetMontagePlayRate()
{
	UAnimMontage* montage;

	switch (mSkillState)
	{
	case ESkillState::ESS_Sprint:
		montage = *mMontages.Find(FName("Sprint"));
		mAnimInst->Montage_SetPlayRate(montage, 0.1f);
		break;
	case ESkillState::ESS_Ribbon:
		montage = *mMontages.Find(FName("Ribbon"));
		mAnimInst->Montage_SetPlayRate(montage, 1.5f);
		break;
	}
}

void AValkyrie::SpawnEffect()
{
	FActorSpawnParameters	SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	switch (mSkillState)
	{
	case ESkillState::ESS_Sprint:
	{
		AValkyrieLightning* niagara = GetWorld()->SpawnActor<AValkyrieLightning>(
			GetActorLocation() + GetActorForwardVector() * 300.f,
			FRotator::ZeroRotator,
			SpawnParam
			);
		niagara->SetParticle(TEXT("NiagaraSystem'/Game/StylizedVFX-Atacks/Particles/NS_LightningAttactOnPoint_2.NS_LightningAttactOnPoint_2'"));
		niagara->SetNiagaraScale(FVector(0.1f));
	}
		break;

	case ESkillState::ESS_Ribbon:
	{
		AParticleNiagara* niagara = GetWorld()->SpawnActor<AParticleNiagara>(
			GetActorLocation(),
			FRotator::ZeroRotator,
			SpawnParam
			);
		niagara->SetParticle(TEXT("NiagaraSystem'/Game/Hack_And_Slash_FX/VFX_Niagara/Slashes/NS_Demon_Slash.NS_Demon_Slash'"));
	}
	break;

	case ESkillState::ESS_Slash:
	{
		FRotator rotation = FRotator(0.f, GetControlRotation().Yaw-90.f, 0.f);
		AValkyrieSlash* slash = GetWorld()->SpawnActor<AValkyrieSlash>(
			GetActorLocation() + GetActorForwardVector() * 100.f,
			rotation,
			SpawnParam
			);
		slash->SetParticle(TEXT("NiagaraSystem'/Game/Hack_And_Slash_FX/VFX_Niagara/Slashes/NS_Fire_Slash.NS_Fire_Slash'"));
		slash->SetDirection(GetActorForwardVector());
	}
	break;
	}
}

void AValkyrie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction<AValkyrie>(TEXT("Skill1"), EInputEvent::IE_Released,
		this, &AValkyrie::Skill1Key);
	PlayerInputComponent->BindAction<AValkyrie>(TEXT("Skill2"), EInputEvent::IE_Released,
		this, &AValkyrie::Skill2Key);
	PlayerInputComponent->BindAction<AValkyrie>(TEXT("Skill3"), EInputEvent::IE_Released,
		this, &AValkyrie::Skill3Key);
}

void AValkyrie::UnequipSword()
{
	if (mWeapon)
	{
		mWeapon->Equip(GetMesh(), TEXT("UnEquipSword"), this, this);
		mWeapon->SetCollisionOnOff(false);
		mCharacterState = ECharacterState::ECS_Unequipped;
		mDirection = 0.f;
	}
}

void AValkyrie::SetAnimDelegate()
{
	mAnimInst->mOnNextAttackCheck.AddLambda([this]() -> void {
		mIsCanNextAttack = false;
		if (mIsAttackInputOn)
		{
			NormalAttackStart();
			PlayMontage(FName("Attack"), FName(*FString::Printf(TEXT("Attack%d"), mCurrentAttackIndex)));
		}
	});

	mAnimInst->mOnAttackEnd.AddLambda([this]()->void {
		mIsAttacking = false;
		mIsJumpAttack = false;
		NormalAttackEnd();
		if (mWeapon)
		{
			mWeapon->ClearIgnoreActors();
			mWeapon->SetTrailOnOff(false);
		}
	});

	mAnimInst->mOnAttackCheckStart.AddLambda([this]() -> void {
		if (mWeapon)
		{
			mWeapon->SetTrailOnOff(true);
			mWeapon->SetCollisionOnOff(true);
		}
	});

	mAnimInst->mOnLaunch.AddLambda([this]() -> void {
		GetCharacterMovement()->BrakingFrictionFactor = 0.f;

		if (mSkillState == ESkillState::ESS_Sprint)
		{
			LaunchCharacter(FVector(0.f, 0.f, 500.f), true, true);
			GetWorld()->GetTimerManager().SetTimer(mTimer, this, &AValkyrie::SetMontagePlayRate, 0.7f, false);
		}
		else if (mSkillState == ESkillState::ESS_Ribbon)
		{
			LaunchCharacter(FVector(0.f, 0.f, 500.f), true, true);
			SetMontagePlayRate();
		}
	});

	mAnimInst->mSkillEnd.AddLambda([this]() -> void {
		if (mSkillState == ESkillState::ESS_Sprint)
		{
			mCameraOne->SetActive(false);
			mCameraComp->SetActive(true);
			mSpringArmComp->TargetArmLength = 400.f;
			GetCharacterMovement()->BrakingFrictionFactor = 2.f;
		}
		else if (mSkillState == ESkillState::ESS_Ribbon)
		{
			GetCharacterMovement()->BrakingFrictionFactor = 2.f;
		}
	});

	mAnimInst->mChangeCamera.AddLambda([this]() -> void {
		UAnimMontage* montage;

	if (mSkillState == ESkillState::ESS_Sprint)
	{
		montage = *mMontages.Find(FName("Sprint"));
		mAnimInst->Montage_SetPlayRate(montage, 0.5f);

		if (APlayerController* controller = UGameplayStatics::GetPlayerController(this, 0))
		{
			mCameraComp->SetActive(false);
			mCameraOne->SetActive(true);
			mSpringArmComp->TargetArmLength = 150.f;
		}
	}
		});

	mAnimInst->mJumpAttackEnable.AddLambda([this]() -> void {
		mIsJumpAttack = true;
	});

	mAnimInst->mOnJumpEnd.AddLambda([this]() -> void {
		mIsJumpAttack = false;
		mActionState = EActionState::EAS_Idle;
		mJumpAttackIndex = -1;
	});
}