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
#include "../Particle/ValkyrieDemonSlash.h"
#include "../Particle/ValkyrieBlinkFire.h"
#include "../Skill/FresnelActor.h"
#include "CharacterStatComponent.h"
#include "TargetingComponent.h"
#include "Components/TimelineComponent.h"
#include "../Monster/Monster.h"
#include "ValkyriePlayerState.h"
#include "../AbilitySystem/AGAbilitySystemComponent.h"
#include "AGPlayerController.h"
#include "../Widget/HUD/AGHUD.h"
#include "../AbilitySystem/AGAttributeSet.h"

AValkyrie::AValkyrie()
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


	static ConstructorHelpers::FClassFinder <UValkyrieAnimInstance>    animInst(TEXT("AnimBlueprint'/Game/Blueprints/Valkyrie_BP/Animations/ABP_Valkyrie.ABP_Valkyrie_C'"));
	if (animInst.Succeeded())
	{
		GetMesh()->SetAnimClass(animInst.Class);
	}



	UAnimMontage* montage;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> equipMontage(
		TEXT("AnimMontage'/Game/Blueprints/Valkyrie_BP/Animations/Montages/AM_Valkyrie_Equip.AM_Valkyrie_Equip'")
	);
	if (equipMontage.Succeeded())
	{
		montage = equipMontage.Object;
	}
	mMontages.Add(FName("Equip"), montage);

	UAnimMontage* montage2;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> attackMontage(TEXT("AnimMontage'/Game/Blueprints/Valkyrie_BP/Animations/Montages/AM_Valkyrie_Attack.AM_Valkyrie_Attack'"));
	if (attackMontage.Succeeded())
	{
		montage2 = attackMontage.Object;
	}
	mMontages.Add(FName("Attack"), montage2);

	UAnimMontage* montage3;
	//static ConstructorHelpers::FObjectFinder<UAnimMontage> sprintMontage(TEXT("AnimMontage'/Game/Blueprints/Valkyrie_BP/Animations/Montages/AM_Valkyrie_Sprint.AM_Valkyrie_Sprint'"));
	//static ConstructorHelpers::FObjectFinder<UAnimMontage> sprintMontage(TEXT("AnimMontage'/Game/Blueprints/Valkyrie_BP/Animations/Montages/AM_Valkyrie_Sprint_2.AM_Valkyrie_Sprint_2'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> sprintMontage(TEXT("AnimMontage'/Game/Blueprints/Valkyrie_BP/Animations/Montages/NewAnimMontage.NewAnimMontage'"));
	if (sprintMontage.Succeeded())
	{
		montage3 = sprintMontage.Object;
	}
	mMontages.Add(FName("Sprint"), montage3);

	UAnimMontage* montage4;
	//static ConstructorHelpers::FObjectFinder<UAnimMontage> ribbonMontage(TEXT("AnimMontage'/Game/Blueprints/Valkyrie_BP/Animations/Montages/AM_Valkyrie_Ribbon.AM_Valkyrie_Ribbon'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ribbonMontage(TEXT("AnimMontage'/Game/Blueprints/Valkyrie_BP/Animations/Montages/AM_Valkyrie_Ribbon2.AM_Valkyrie_Ribbon2'"));
	if (ribbonMontage.Succeeded())
	{
		montage4 = ribbonMontage.Object;
	}
	mMontages.Add(FName("Ribbon"), montage4);

	UAnimMontage* montage5;
	//static ConstructorHelpers::FObjectFinder<UAnimMontage> slashMontage(TEXT("AnimMontage'/Game/Blueprints/Valkyrie_BP/Animations/Montages/AM_Valkyrie_Slash.AM_Valkyrie_Slash'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> slashMontage(TEXT("AnimMontage'/Game/Blueprints/Valkyrie_BP/Animations/Montages/AM_Valkyrie_SlashSkills.AM_Valkyrie_SlashSkills'"));
	if (slashMontage.Succeeded())
	{
		montage5 = slashMontage.Object;
	}
	mMontages.Add(FName("Slash"), montage5);

	UAnimMontage* montage6;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> doubleJumpMontage(TEXT("AnimMontage'/Game/Assets/Character/Valkyrie/Animation/Jump/AM_Double_Jump.AM_Double_Jump'"));
	if (doubleJumpMontage.Succeeded())
	{
		montage6 = doubleJumpMontage.Object;
	}
	mMontages.Add(FName("DoubleJump"), montage6);

	UAnimMontage* montage7;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> hit_right(TEXT("AnimMontage'/Game/Blueprints/Player/Animation/Hit/AM_Valkyrie_Hit_Right.AM_Valkyrie_Hit_Right'"));
	if (hit_right.Succeeded())
	{
		montage7 = hit_right.Object;
	}
	mMontages.Add(FName("hit_right"), montage7);

	UAnimMontage* montage8;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> hit_left(TEXT("AnimMontage'/Game/Blueprints/Player/Animation/Hit/AM_Valkyrie_Hit_Left.AM_Valkyrie_Hit_Left'"));
	if (hit_left.Succeeded())
	{
		montage8 = hit_left.Object;
	}
	mMontages.Add(FName("hit_left"), montage8);

	UAnimMontage* montage9;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> hit_front(TEXT("AnimMontage'/Game/Blueprints/Player/Animation/Hit/AM_Valkyrie_Hit_Front.AM_Valkyrie_Hit_Front'"));
	if (hit_front.Succeeded())
	{
		montage9 = hit_front.Object;
	}
	mMontages.Add(FName("hit_front"), montage9);

	UAnimMontage* montage10;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> hit_back(TEXT("AnimMontage'/Game/Blueprints/Player/Animation/Hit/AM_Valkyrie_Hit_Back.AM_Valkyrie_Hit_Back'"));
	if (hit_back.Succeeded())
	{
		montage10 = hit_back.Object;
	}
	mMontages.Add(FName("hit_back"), montage10);

	UAnimMontage* montage11;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> hard_attack(TEXT("AnimMontage'/Game/Blueprints/Valkyrie_BP/Animations/Montages/AM_Valkyrie_HardAttack.AM_Valkyrie_HardAttack'"));
	if (hard_attack.Succeeded())
	{
		montage11 = hard_attack.Object;
	}
	mMontages.Add(FName("HardAttack"), montage11);


	mAttackMaxIndex = 4;
	NormalAttackEnd();


	mMotionWarpComp = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpComp"));



	mTempCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TempCamera"));
	mTempCameraComp->SetActive(false);
	mTempCameraComp->SetupAttachment(GetMesh());
	/*mTempCameraComp->SetRelativeLocation(FVector(20.f, -100.f, 180.f));
	mTempCameraComp->SetRelativeRotation(FRotator(-20.f, 90.f, 0.f));*/
	mTempCameraComp->SetRelativeLocation(FVector(-130.f, -145.f, 225.f));
	mTempCameraComp->SetRelativeRotation(FRotator(-35.f, 45.f, 10.f));
	mTempCameraComp->bAutoActivate = false;


	mCameraCompRef = CreateDefaultSubobject<UCameraComponent>(TEXT("mCameraCompRef"));
	mCameraCompRef->SetupAttachment(GetCapsuleComponent());
	mCameraCompRef->SetRelativeLocation(FVector(-400.f, 0.f, 0.f));
	mCameraCompRef->SetActive(false);
	mCameraCompRef->bAutoActivate = false;
	tempLocation = FVector(0.f);

	JumpMaxCount = 2;
	GetCharacterMovement()->JumpZVelocity = 500.f;


	mFresnelInfo.mFresnelEnable = false;
	mFresnelInfo.mFresnelTime = 0.f;
	mFresnelInfo.mFresnelTimeEnd = 1.f;
	mFresnelInfo.mFresnelCreateTime = 0.f;
	mFresnelInfo.mFresnelCreateTimeEnd = 0.4f;


	static ConstructorHelpers::FClassFinder<UCameraShakeBase>	cameraShake(TEXT("Blueprint'/Game/Blueprints/CameraShake/CS_PlayerNormalAttack.CS_PlayerNormalAttack_C'"));

	if (cameraShake.Succeeded())
		mCameraShake = cameraShake.Class;



	mTargetingComp = CreateDefaultSubobject<UTargetingComponent>(TEXT("TargetingComp"));
	mTargetingComp->SetOwner(this);





	mTimeLineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("mTimelineComp"));
	mCrouchTimeLineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("mCrouchTimeLineComp"));


	static ConstructorHelpers::FObjectFinder<UCurveFloat> curve(TEXT("CurveFloat'/Game/Blueprints/GameMode/BP_CameraCurve.BP_CameraCurve'"));
	if (curve.Succeeded())
	{
		mTimeLineCurve = curve.Object;
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("AnimBlueprint'/Game/Blueprints/Valkyrie_BP/Animations/ABP_Valkyrie.ABP_Valkyrie_C'"));
	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}

	mCrouchCapsuleHalfHeight = 60.f;
	mCapsuleHalfHeight = 88.f;

	
}

void AValkyrie::BeginPlay()
{
	Super::BeginPlay();

	mCameraComp->SetAspectRatio(1.777778);
	mCameraComp->bConstrainAspectRatio = true;

	mTempCameraComp->SetAspectRatio(1.777778);
	mTempCameraComp->bConstrainAspectRatio = true;

	mAnimInst = Cast<UValkyrieAnimInstance>(GetMesh()->GetAnimInstance());

	FActorSpawnParameters	SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	mWeapon = GetWorld()->SpawnActor<ASword>(ASword::StaticClass(), SpawnParam);
	mWeapon->SetSkeletalMesh(TEXT("SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_HeroSword11/SK_Blade_HeroSword11.SK_Blade_HeroSword11'"));
	mWeapon->Equip(GetMesh(), TEXT("UnEquipSword"), this, this);
	mWeapon->SetCollisionOnOff(false);
	mWeapon->SetSwordOwner(this);

	SetAnimDelegate();

	mCurveUpdateDelegate.BindUFunction(this, FName("CurveUpdate"));
	mCrouchCurveUpdateDelegate.BindUFunction(this, FName("CrouchCurveUpdate"));
	mTimelineFinishDelegate.BindUFunction(this, FName("TimeLineFinish"));

	// timeline update
	mTimeLineComp->AddInterpFloat(mTimeLineCurve, mCurveUpdateDelegate);
	mCrouchTimeLineComp->AddInterpFloat(mTimeLineCurve, mCrouchCurveUpdateDelegate);
	// timeline finish
	mTimeLineComp->SetTimelineFinishedFunc(mTimelineFinishDelegate);
	// timeline length
	mTimeLineComp->SetTimelineLength(0.5f);

	//GetCapsuleComponent()->bHiddenInGame = false;

	GetCharacterMovement()->MaxAcceleration = 300.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 100.f;
}

void AValkyrie::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AValkyrie::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the server.
	InitAbilityActorInfo();
}

void AValkyrie::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the client.
	InitAbilityActorInfo();
}

void AValkyrie::InitAbilityActorInfo()
{
	AValkyriePlayerState* state = GetPlayerState<AValkyriePlayerState>();
	check(state);
	state->GetAbilitySystemComponent()->InitAbilityActorInfo(state, this);
	Cast<UAGAbilitySystemComponent>(state->GetAbilitySystemComponent())->AbilityActorInfoSet();

	mAbilitySystemComp = state->GetAbilitySystemComponent();
	mAttributeSet = state->GetAttributeSet();

	// Widget과 연결
	if (AAGPlayerController* pc = Cast<AAGPlayerController>(GetController()))
	{
		if (AAGHUD* hud = Cast<AAGHUD>(pc->GetHUD()))
		{
			hud->InitMainWidget(pc, state, mAbilitySystemComp, mAttributeSet);
		}
	}
}


void AValkyrie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (mFresnelInfo.mFresnelEnable)
		SpawnFresnel();
}

void AValkyrie::PlayMontage(FName _montageName, FName _sectionName)
{
	UAnimMontage* montage = *mMontages.Find(_montageName);
	mAnimInst->Montage_Play(montage);
	if (_sectionName != "")
		mAnimInst->Montage_JumpToSection(_sectionName, montage);
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

void AValkyrie::JumpKey()
{
	if (mIsJump)
	{
		mIsJump = false;
		LaunchCharacter(FVector(0.f, 0.f, 700.f), true, true);
		PlayMontage(FName("DoubleJump"));
	}
	else
	{
		Jump();
		mIsJump = true;
	}
}

void AValkyrie::Skill1Key()
{
	mSkillState = ESkillState::ESS_Sprint;

	AValkyriePlayerState* state = GetPlayerState<AValkyriePlayerState>();
	UAGAttributeSet* attributeSet = Cast<UAGAttributeSet>(state->GetAttributeSet());
	attributeSet->SetmMp(attributeSet->GetmMp() - 10.f);

	mWeapon->SetCollisionOnOff(false);


	TArray<AActor*> targetActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AValkyrie::GetClass(), targetActors);
	
	FVector location = GetActorLocation();
	FVector targetLocation = FVector();
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	TArray<AActor*> ignoreActors;
	TArray<AActor*> outActors;

	objectTypes.Add(UEngineTypes::ConvertToObjectType(
		ECollisionChannel::ECC_GameTraceChannel3)
	);
	ignoreActors.Add(this);

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
			targetLocation = outActors[0]->GetActorLocation();
		}
	}

	FMotionWarpingTarget mwt;
	mwt.Transform.SetLocation(targetLocation);
	mwt.Transform.SetRotation(UKismetMathLibrary::FindLookAtRotation(location, targetLocation).Quaternion());
	mMotionWarpComp->AddOrUpdateWarpTarget(FName("SprintTarget"), mwt);
	PlayMontage(FName("Sprint"));
}

void AValkyrie::Skill2Key()
{
	mSkillState = ESkillState::ESS_Ribbon;
	
	AValkyriePlayerState* state = GetPlayerState<AValkyriePlayerState>();
	UAGAttributeSet* attributeSet = Cast<UAGAttributeSet>(state->GetAttributeSet());
	attributeSet->SetmMp(attributeSet->GetmMp() - 10.f);

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
	
	AValkyriePlayerState* state = GetPlayerState<AValkyriePlayerState>();
	UAGAttributeSet* attributeSet = Cast<UAGAttributeSet>(state->GetAttributeSet());
	attributeSet->SetmMp(attributeSet->GetmMp() - 10.f);

	
	mWeapon->SetCollisionOnOff(false);
	

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

		PlayMontage(FName("Slash"), FName(*FString::Printf(TEXT("Slash%d"), ++mSlashSkillIndex)));
	}

	PrintViewport(1.f, FColor::Yellow, FString::Printf(TEXT("index : %d"), mSlashSkillIndex));
}

void AValkyrie::Skill4Key()
{
	mSkillState = ESkillState::ESS_HardAttack;
	PlayMontage(FName("HardAttack"));

	UAnimMontage* montage = *mMontages.Find(FName("HardAttack"));
	mAnimInst->Montage_SetPlayRate(montage, 0.1f);
}

void AValkyrie::TargetingKey()
{
	mTargetingComp->SetTargetLock();
}

void AValkyrie::CrouchKey()
{
	if (!mIsCrouch)
	{
		mIsCrouch = true;
		mCrouchTimeLineComp->ReverseFromEnd();
		GetCharacterMovement()->MaxWalkSpeed = 100.f;

		GetCapsuleComponent()->SetRelativeLocation(FVector(0.f, 0.f, -60.f));
		GetMesh()->SetRelativeLocation(GetMesh()->GetRelativeLocation() + FVector(0.f, 0.f, 30.f));
		GetCapsuleComponent()->SetCapsuleHalfHeight(mCrouchCapsuleHalfHeight);
	}
	else
	{
		mIsCrouch = false;
		mCrouchTimeLineComp->PlayFromStart();
		GetCharacterMovement()->MaxWalkSpeed = 400.f;

		GetCapsuleComponent()->SetRelativeLocation(FVector(0.f, 0.f, 60.f));
		GetMesh()->SetRelativeLocation(GetMesh()->GetRelativeLocation() + FVector(0.f, 0.f, -30.f));
		GetCapsuleComponent()->SetCapsuleHalfHeight(mCapsuleHalfHeight);
	}
}

void AValkyrie::NormalAttackStart()
{
	if (mCurrentAttackIndex == 4)
		mCurrentAttackIndex = 1;	
	mIsCanNextAttack = true;
	mIsAttackInputOn = false;
	mCurrentAttackIndex = FMath::Clamp<int32>(mCurrentAttackIndex + 1, 1, mAttackMaxIndex);
	

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
		mAnimInst->Montage_SetPlayRate(montage, 0.6f);
		break;
	}
}

void AValkyrie::SpawnFresnel()
{
	mFresnelInfo.mFresnelTime += GetWorld()->DeltaTimeSeconds;
	mFresnelInfo.mFresnelCreateTime += GetWorld()->DeltaTimeSeconds;

	if (mFresnelInfo.mFresnelCreateTime >= mFresnelInfo.mFresnelCreateTimeEnd)
	{
		mFresnelInfo.mFresnelCreateTime -= mFresnelInfo.mFresnelCreateTimeEnd;
		//PrintViewport(0.5f, FColor::Black, TEXT("Fresnel"));
		// 잔상 생성
		FActorSpawnParameters	SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AFresnelActor* ghost =
			GetWorld()->SpawnActor<AFresnelActor>(
				GetMesh()->GetComponentLocation(),
				GetMesh()->GetComponentRotation(),
				SpawnParam);

		ghost->SetMesh(mGhostMesh);
		ghost->CopyPoseFromSkeletalComp(GetMesh());
	}

	if (mFresnelInfo.mFresnelTime >= mFresnelInfo.mFresnelTimeEnd)
	{
		mFresnelInfo.mFresnelTime = 0.f;
		mFresnelInfo.mFresnelEnable = false;
	}
}

void AValkyrie::ResetFresnel()
{
	mFresnelInfo.mFresnelEnable = false;
	mFresnelInfo.mFresnelTime = 0.f;
	mFresnelInfo.mFresnelTimeEnd = 1.f;
	mFresnelInfo.mFresnelCreateTime = 0.f;
	mFresnelInfo.mFresnelCreateTimeEnd = 0.4f;
}

void AValkyrie::TimeLineFinish()
{
	if (mToCameraComp)
	{
		FAttachmentTransformRules attachRules(EAttachmentRule::SnapToTarget, true);
		mCameraComp->AttachToComponent(mSpringArmComp, attachRules);
	}
	else
	{
		mCameraComp->SetActive(false);
		mTempCameraComp->SetActive(true);
		mCameraCompRef->SetActive(false);
	}
}

void AValkyrie::CurveUpdate(float value)
{
	if (value < 0.f)
		return;
	FTransform newTransform = FTransform();

	if (mToCameraComp)
	{
		newTransform = UKismetMathLibrary::TLerp(mCameraCompTrans, mTempCameraTrans, value);
		mCameraComp->SetWorldTransform(newTransform);
	}
	else
	{
		newTransform = UKismetMathLibrary::TLerp(mCameraCompTrans, mTempCameraTrans, value);
		mCameraComp->SetRelativeTransform(newTransform);
	}

	FRotator newControllerRotator = FRotator(0.f);
	newControllerRotator.Roll = 0.f;
	newControllerRotator.Pitch = mTempCameraComp->GetComponentRotation().Pitch;
	newControllerRotator.Yaw = newTransform.Rotator().Yaw;
	APlayerController* controller = UGameplayStatics::GetPlayerController(this, 0);
	controller->SetControlRotation(newControllerRotator);
}

void AValkyrie::CrouchCurveUpdate(float value)
{
	mCameraComp->SetRelativeLocation(FVector(0.f, 0.f, (FMath::Lerp(0.f, 30.f, value))));
}

void AValkyrie::CameraSwitch(bool _value)
{
	FAttachmentTransformRules attachRules(EAttachmentRule::KeepWorld, true);
	FDetachmentTransformRules detachRules(EDetachmentRule::KeepWorld, true);
	mTempCameraComp->DetachFromComponent(detachRules);
	mTempCameraTrans = mTempCameraComp->GetRelativeTransform();
	mTempCameraComp->AttachToComponent(GetMesh(), attachRules);

	if (_value)
	{
		mToCameraComp = false;
		mCameraComp->DetachFromComponent(detachRules);
		mCameraCompTrans = mCameraComp->GetRelativeTransform();
		mTimeLineComp->PlayFromStart();
	}
	else
	{
		mToCameraComp = true;

		mCameraCompTrans = mCameraCompRef->GetComponentTransform();
		mTempCameraTrans = mTempCameraComp->GetComponentTransform();

		mCameraComp->SetActive(true);
		mTempCameraComp->SetActive(false);
		mCameraCompRef->SetActive(false);

		mTimeLineComp->ReverseFromEnd();
	}
}

void AValkyrie::SpawnEffect()
{
	FActorSpawnParameters	SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector location = FVector();

	switch (mActionState)
	{
	case EActionState::EAS_Attack:
		AValkyrieBlinkFire* slash = GetWorld()->SpawnActor<AValkyrieBlinkFire>(
			GetActorLocation(),
			GetActorRotation(),
			SpawnParam
		);
		slash->SetParticle(TEXT("NiagaraSystem'/Game/BlinkAndDashVFX/VFX_Niagara/NS_Blink_Fire.NS_Blink_Fire'"));
		return;
		break;
	}
	

	switch (mSkillState)
	{
	case ESkillState::ESS_Sprint:
	{
		location = GetActorLocation();
		location.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		AValkyrieLightning* niagara = GetWorld()->SpawnActor<AValkyrieLightning>(
			location,
			FRotator::ZeroRotator,
			SpawnParam
			);
		//niagara->SetParticle(TEXT("NiagaraSystem'/Game/StylizedVFX-Atacks/Particles/NS_LaserAttack.NS_LaserAttack'"));
		//niagara->SetParticle(TEXT("NiagaraSystem'/Game/NiagaraMagicalSlashes/Fx/Slashes/NS_Slash_GuDef_01.NS_Slash_GuDef_01'"));
		niagara->SetParticle(TEXT("NiagaraSystem'/Game/NiagaraMagicalSlashes/Fx/Slashes/NS_SlashRing_05.NS_SlashRing_05'"));
		//niagara->SetNiagaraScale(FVector(0.1f));
	}
		break;

	case ESkillState::ESS_Ribbon:
	{
		AValkyrieDemonSlash* niagara = GetWorld()->SpawnActor<AValkyrieDemonSlash>(
			GetActorLocation(),
			FRotator::ZeroRotator,
			SpawnParam
			);
		niagara->SetParticle(TEXT("NiagaraSystem'/Game/Hack_And_Slash_FX/VFX_Niagara/Slashes/NS_Demon_Slash.NS_Demon_Slash'"));
	}
	break;

	case ESkillState::ESS_Slash:
	{
		if (mSlashSkillIndex == 0)
		{
			AValkyrieSlash* slash = GetWorld()->SpawnActor<AValkyrieSlash>(
				GetActorLocation()/* + GetActorForwardVector() * 100.f*/,
				GetActorRotation(),
				SpawnParam
			);
			slash->SetParticle(TEXT("NiagaraSystem'/Game/NiagaraMagicalSlashes/Fx/Slashes/NS_Col_SL_12.NS_Col_SL_12'"));
			slash->SetDirection(GetActorForwardVector());
		}
		else if (mSlashSkillIndex == 1)
		{
			AValkyrieSlash* slash = GetWorld()->SpawnActor<AValkyrieSlash>(
				GetActorLocation(),
				GetActorRotation(),
				SpawnParam
			);
			slash->SetParticle(TEXT("NiagaraSystem'/Game/NiagaraMagicalSlashes/Fx/Slashes/NS_Col_SL_13.NS_Col_SL_13'"));
			slash->SetDirection(GetActorForwardVector());
		}
		else if (mSlashSkillIndex == 2)
		{
			AValkyrieSlash* slash = GetWorld()->SpawnActor<AValkyrieSlash>(
				GetActorLocation(),
				GetActorRotation(),
				SpawnParam
			);
			slash->SetParticle(TEXT("NiagaraSystem'/Game/NiagaraMagicalSlashes/Fx/Slashes/NS_Blade_Sl_10.NS_Blade_Sl_10'"));
			slash->SetDirection(GetActorForwardVector());
		}
		else if (mSlashSkillIndex == 3)
		{
			FVector forward = GetActorForwardVector();
			forward.Y = 0.f;
			forward.Z = 0.f;
			AValkyrieSlash* slash = GetWorld()->SpawnActor<AValkyrieSlash>(
				GetActorLocation(),
				FRotator(0.f, GetControlRotation().Yaw - 90.f, 0.f),
				SpawnParam
			);
			slash->SetActorScale3D(FVector(0.5f));
			slash->SetParticle(TEXT("NiagaraSystem'/Game/NiagaraMagicalSlashes/Fx/Slashes/NS_Slash_HeavyAb_03.NS_Slash_HeavyAb_03'"));
			slash->SetDirection(GetActorForwardVector());
		}
	}
	break;

	case ESkillState::ESS_HardAttack:
	{
		CameraSwitch(true);
		AValkyrieSlash* slash = GetWorld()->SpawnActor<AValkyrieSlash>(
			GetActorLocation() + GetActorForwardVector() * 50.f,
			GetActorRotation(),
			SpawnParam
		);
		slash->SetParticle(TEXT("NiagaraSystem'/Game/StylizedVFX-Atacks/Particles/NS_SwordAttack.NS_SwordAttack'"));
	}
	break;
	}


}

void AValkyrie::Delay(float _customTimeDilation, float _timeRate, bool _isLoop)
{
	CustomTimeDilation = _customTimeDilation;
	GetWorld()->GetTimerManager().SetTimer(mTimer, FTimerDelegate::CreateLambda([&]() 
		{
			CustomTimeDilation = 1.f;
		}), 
		_timeRate, 
		_isLoop
		);
	GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(mCameraShake);
}

void AValkyrie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction<AValkyrie>(TEXT("Skill1"), EInputEvent::IE_Pressed,
		this, &AValkyrie::Skill1Key);
	PlayerInputComponent->BindAction<AValkyrie>(TEXT("Skill2"), EInputEvent::IE_Pressed,
		this, &AValkyrie::Skill2Key);
	PlayerInputComponent->BindAction<AValkyrie>(TEXT("Skill3"), EInputEvent::IE_Pressed,
		this, &AValkyrie::Skill3Key);
	PlayerInputComponent->BindAction<AValkyrie>(TEXT("Skill4"), EInputEvent::IE_Pressed,
		this, &AValkyrie::Skill4Key);
	PlayerInputComponent->BindAction<AValkyrie>(TEXT("Targeting"), EInputEvent::IE_Pressed,
		this, &AValkyrie::TargetingKey);
	
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

void AValkyrie::GetHit(const FVector& _impactPoint)
{
	if (mSkillState != ESkillState::ESS_None || mActionState == EActionState::EAS_Attack)
		return;
	FVector position = GetActorLocation();
	FVector impactPosition = FVector(_impactPoint.X, _impactPoint.Y, position.Z);
	FVector direction = (impactPosition - position).GetSafeNormal();

	// Forward * direction = |forward| * |direction| * cos(theta)
	// 근데 |forward|, |direction| 이 2개는 크기가 1이므로 
	// 내적의 결과는 cos(theta)가 된다.
	float innerProduct = FVector::DotProduct(GetActorForwardVector(), direction);
	// theta값을 얻는다
	// 라디안과 삼각함수는 일반적으로 각도 대신 라디안을 사용한다
	float degree = UKismetMathLibrary::Acos(innerProduct);
	// 라디안을 실제 각도(몇도)로 변환한다
	degree = FMath::RadiansToDegrees(degree);

	// 오른쪽/왼쪽 구분
	// outProdouct가 위를 향하고 있다 -> 오른쪽 (양수)
	// 아래 -> 왼쪽 (음수)
	FVector outProduct = FVector::CrossProduct(GetActorForwardVector(), direction);
	float sign = UKismetMathLibrary::SignOfFloat(outProduct.Z);

	float angle = sign * degree;



	FName angleString = TEXT("");

	// 오른쪽.
	if (angle >= 0.f)
	{
		if (degree >= 50.f && angle <= 130.f)
			angleString = TEXT("hit_right");
		else if (degree < 50.f)
			angleString = TEXT("hit_front");
		else
			angleString = TEXT("hit_back");
	}

	// 왼쪽
	else if (angle < 0.f)
	{
		if (degree <= -50.f && angle >= -130.f)
			angleString = TEXT("hit_left");
		else if (degree > -50.f)
			angleString = TEXT("hit_front");
		else
			angleString = TEXT("hit_back");
	}

	PlayMontage(angleString);

	//mAnimInst->SetHitDirection(angleString);
}


void AValkyrie::SetAnimDelegate()
{
	mAnimInst->mOnAttackEnd.AddLambda([this]()->void {
		CustomTimeDilation = 1.f;
		mIsAttacking = false;
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

	mAnimInst->mOnNextAttackCheck.AddLambda([this]() -> void {
		mIsCanNextAttack = false;
	if (mIsAttackInputOn)
	{
		NormalAttackStart();
		PlayMontage(FName("Attack"), FName(*FString::Printf(TEXT("Attack%d"), mCurrentAttackIndex)));
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
			LaunchCharacter(FVector(0.f, 0.f, 700.f), true, true);
			SetMontagePlayRate();
		}
	});

	mAnimInst->mSkillEnd.AddLambda([this]() -> void {
		if (mSkillState == ESkillState::ESS_Sprint)
		{
			//CameraSwitch(false);
			GetCharacterMovement()->BrakingFrictionFactor = 2.f;
		}
		else if (mSkillState == ESkillState::ESS_Ribbon)
		{
			GetCharacterMovement()->BrakingFrictionFactor = 2.f;
			ResetFresnel();
		}
		else if (mSkillState == ESkillState::ESS_Slash)
		{
			PrintViewport(2.f, FColor::Red, FString("Slash End"));
			mSlashSkillIndex = -1;
			mIsNextSlashEnable = false;
			mIsNextSlashInput = false;
			mIsSlash = false;
		}
		else if (mSkillState == ESkillState::ESS_HardAttack)
		{

			CameraSwitch(false);
			UAnimMontage* montage = *mMontages.Find(FName("HardAttack"));
			mAnimInst->Montage_SetPlayRate(montage, 1.f);
		}
		mSkillState = ESkillState::ESS_None;

	});

	mAnimInst->mChangeCamera.AddLambda([this]() -> void {
		UAnimMontage* montage;

	if (mSkillState == ESkillState::ESS_Sprint)
	{
		montage = *mMontages.Find(FName("Sprint"));
		mAnimInst->Montage_SetPlayRate(montage, 0.1f);
		CameraSwitch(true);

	}
	});

	mAnimInst->mOnJumpEnd.AddLambda([this]() -> void {
		mActionState = EActionState::EAS_Idle;
		mIsJump = false;
	});

	mAnimInst->mSpawnFresnel.AddLambda([this]() -> void {
		if (mSkillState == ESkillState::ESS_Ribbon)
		{
			mFresnelInfo.mFresnelEnable = true;
			mFresnelInfo.mFresnelCreateTimeEnd = 0.1f;
			SpawnFresnel();
		}
		});

	mAnimInst->mResetFresnel.AddLambda([this]() -> void {
		if (mSkillState == ESkillState::ESS_Ribbon)
		{
			mFresnelInfo.mFresnelEnable = false;
			ResetFresnel();
		}
		});

	mAnimInst->mOnDelay.AddLambda([this]() -> void {
		CustomTimeDilation = 0.2f;
		GetWorld()->GetTimerManager().SetTimer(mTimer, FTimerDelegate::CreateLambda([&]() {
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(mCameraShake);
		CustomTimeDilation = 1.f;
		}), 0.2f, false);
		});

	mAnimInst->mOnHitEnd.AddLambda([this]()-> void {
		mWeapon->SetTrailOnOff(false);
		});

	mAnimInst->mOnSlashEnable.AddLambda([this]()-> void {

		mIsNextSlashEnable = false;
		if (mIsNextSlashInput)
		{
			//NormalAttackStart();

			mIsNextSlashEnable = true;
			mIsNextSlashInput = false;
			PlayMontage(FName("Slash"), FName(*FString::Printf(TEXT("Slash%d"), ++mSlashSkillIndex)));
		}

		
		});
}