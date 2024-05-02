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
#include "Shield.h"
#include "../AGGameplayTags.h"
//#include "../AbilitySystem/Ability/ValkyrieNormalAttack.h"
#include "../Skill/Valkyrie/ValkyrieSprint.h"
#include "../Skill/Valkyrie/ValkyrieRange.h"
#include "../Skill/Valkyrie/ValkyrieContinuousSlash.h"
#include "../Skill/Valkyrie/ValkyrieNormalAttack.h"
#include "../Skill/Valkyrie/ValkyrieFallingSword.h"

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

	// init AGAttributes by GameplayEffect
	static ConstructorHelpers::FClassFinder<UGameplayEffect> defaultFirstAttributes(TEXT("Blueprint'/Game/Blueprints/GameplayEffects/ActorInitAttributes/GE_ValkyrieFirstAttributes.GE_ValkyrieFirstAttributes_C'"));
	if (defaultFirstAttributes.Succeeded())
	{
		mDefaultFirstAttributes = defaultFirstAttributes.Class;
	}
	static ConstructorHelpers::FClassFinder<UGameplayEffect> defaultSecondAttributes(TEXT("Blueprint'/Game/Blueprints/GameplayEffects/ActorInitAttributes/GE_ValkyrieSecond.GE_ValkyrieSecond_C'"));
	if (defaultSecondAttributes.Succeeded())
	{
		mDefaultSecondAttributes = defaultSecondAttributes.Class;
	}
	static ConstructorHelpers::FClassFinder<UGameplayEffect> defaultVitalAttributes(TEXT("Blueprint'/Game/Blueprints/GameplayEffects/ActorInitAttributes/GE_ValkyrieVitalAttributes.GE_ValkyrieVitalAttributes_C'"));
	if (defaultVitalAttributes.Succeeded())
	{
		mDefaultVitalAttributes = defaultVitalAttributes.Class;
	}





	static ConstructorHelpers::FClassFinder<UGameplayAbility> testGameplayAbility(TEXT("Blueprint'/Game/Blueprints/AbilitySystem/GameplayAbility/GA_ValkyrieNA.GA_ValkyrieNA_C'"));
	TSubclassOf<UGameplayAbility> ga;
	if (testGameplayAbility.Succeeded())
	{
		ga = testGameplayAbility.Class;
	}
	mStartupAbilites.Add(ga);

	
	//TSubclassOf<UGameplayAbility> gaa = UValkyrieNormalAttack::StaticClass();
	//mStartupAbilites.Add(gaa);

	TSubclassOf<AAGSkillActor> sprint = AValkyrieSprint::StaticClass();
	mSkillmap.Add(EValkyrieSkill::EVS_Sprint, sprint);

	TSubclassOf<AAGSkillActor> range = AValkyrieRange::StaticClass();
	mSkillmap.Add(EValkyrieSkill::EVS_Range, range);

	TSubclassOf<AAGSkillActor> slash = AValkyrieContinuousSlash::StaticClass();
	mSkillmap.Add(EValkyrieSkill::EVS_Slash, slash);

	TSubclassOf<AAGSkillActor> normalAttack = AValkyrieNormalAttack::StaticClass();
	mSkillmap.Add(EValkyrieSkill::EVS_NormalAttack, normalAttack);

	TSubclassOf<AAGSkillActor> fallingSword = AValkyrieFallingSword::StaticClass();
	mSkillmap.Add(EValkyrieSkill::EVS_FallingSword, fallingSword);

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


	mGuardShield = GetWorld()->SpawnActor<AShield>(AShield::StaticClass(), SpawnParam);
	mGuardShield->SetSkeletalMesh(TEXT("SkeletalMesh'/Game/Goblin/Mesh/Weapons/SK_Shield_A.SK_Shield_A'"));
	mGuardShield->Equip(GetMesh(), TEXT("Guard"), this, this);


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

void AValkyrie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (mFresnelInfo.mFresnelEnable)
		SpawnFresnel();


	//PrintViewport(0.5f, FColor::Red, FString::Printf(TEXT("x: %f, y: %f, z: %f"),
	//	GetActorLocation().X,
	//	GetActorLocation().Y,
	//	GetActorLocation().Z));
	//PrintAllActionState();
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

	AddCharacterAbilities();
}

void AValkyrie::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the client.
	InitAbilityActorInfo();
}

int32 AValkyrie::GetPlayerLevel()
{
	const AValkyriePlayerState* state = GetPlayerState<AValkyriePlayerState>();
	check(state);
	return state->GetPlayerLevel();
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

	// Default Attribute를 설정한다.
	InitializeDefaultAttributes();
}

void AValkyrie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction<AValkyrie>(TEXT("Skill1"), EInputEvent::IE_Pressed,
		this, &AValkyrie::Skill1Key);
	PlayerInputComponent->BindAction<AValkyrie>(TEXT("Skill2"), EInputEvent::IE_Pressed,
		this, &AValkyrie::Skill2Key);
	PlayerInputComponent->BindAction<AValkyrie>(TEXT("Skill2"), EInputEvent::IE_Released,
		this, &AValkyrie::Skill2KeyUp);
	PlayerInputComponent->BindAction<AValkyrie>(TEXT("Skill2"), EInputEvent::IE_Repeat,
		this, &AValkyrie::Skill2KeyPressing);

	PlayerInputComponent->BindAction<AValkyrie>(TEXT("Skill3"), EInputEvent::IE_Pressed,
		this, &AValkyrie::Skill3Key);
	PlayerInputComponent->BindAction<AValkyrie>(TEXT("Skill4"), EInputEvent::IE_Pressed,
		this, &AValkyrie::Skill4Key);
	PlayerInputComponent->BindAction<AValkyrie>(TEXT("Targeting"), EInputEvent::IE_Pressed,
		this, &AValkyrie::TargetingKey);
	PlayerInputComponent->BindAction<AValkyrie>(TEXT("Shield"), EInputEvent::IE_Pressed,
		this, &AValkyrie::GuardKey);


}

//-------------------------------
// Input Functions
//-------------------------------

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
	//Cast<UAGAbilitySystemComponent>(mAbilitySystemComp)->AbilityInputTagHeld(FAGGameplayTags::Get().InputTag_NormalAttack);


	bool isContainNormalAttackActor = mSkillActorMap.Contains(EValkyrieSkill::EVS_NormalAttack);
	if (!isContainNormalAttackActor)
	{
		FActorSpawnParameters	params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		TSubclassOf<AAGSkillActor> skillActor = *mSkillmap.Find(EValkyrieSkill::EVS_NormalAttack);
		AAGSkillActor* sk = GetWorld()->SpawnActor<AAGSkillActor>(skillActor, GetActorLocation(), GetActorRotation(), params);
		sk->SetOwnerActor(this);
		Cast<AValkyrieNormalAttack>(sk)->EndNormalAttack();
		Cast<AValkyrieNormalAttack>(sk)->InputPressed();
		

		mSkillActorMap.Add(EValkyrieSkill::EVS_NormalAttack, sk);
	}
	else
	{
		AAGSkillActor* skillActor = *mSkillActorMap.Find(EValkyrieSkill::EVS_NormalAttack);
		Cast<AValkyrieNormalAttack>(skillActor)->InputPressed();
	}
}

void AValkyrie::NormalAttackKeyReleased()
{
	Cast<UAGAbilitySystemComponent>(mAbilitySystemComp)->AbilityInputTagReleased(FAGGameplayTags::Get().InputTag_NormalAttack);
}

void AValkyrie::TargetingKey()
{
	mTargetingComp->SetTargetLock();
}

void AValkyrie::CrouchKey()
{
	bool isCrouch = CheckActionState(EActionState2::EAS_Crouch2, false);
	if (!isCrouch)
	{
		SetActionState(EActionState2::EAS_Crouch2, true);

		mCrouchTimeLineComp->ReverseFromEnd();
		GetCharacterMovement()->MaxWalkSpeed = 100.f;

		GetMesh()->SetRelativeLocation(GetMesh()->GetRelativeLocation() + FVector(0.f, 0.f, 30.f));
		GetCapsuleComponent()->SetCapsuleHalfHeight(mCrouchCapsuleHalfHeight);
	}
	else
	{
		SetActionState(EActionState2::EAS_Crouch2, false);

		mCrouchTimeLineComp->PlayFromStart();
		GetCharacterMovement()->MaxWalkSpeed = 400.f;

		GetMesh()->SetRelativeLocation(GetMesh()->GetRelativeLocation() + FVector(0.f, 0.f, -30.f));
		GetCapsuleComponent()->SetCapsuleHalfHeight(mCapsuleHalfHeight);
	}
}

void AValkyrie::GuardKey()
{
	//mIsGuard = !mIsGuard;



	bool isGuard = CheckActionState(EActionState2::EAS_Guard2, false);

	if (isGuard)
	{
		SetActionState(EActionState2::EAS_Guard2, false);
		mGuardShield->SetShieldVisibility(false);
	}
	else
	{
		SetActionState(EActionState2::EAS_Guard2, true);
		mGuardShield->SetShieldVisibility(true);
	}
}

void AValkyrie::JumpKey()
{
	bool isJump = CheckActionState(EActionState2::EAS_Jump2, false);
	Cast<UAGAbilitySystemComponent>(mAbilitySystemComp)->AbilityInputTagHeld(FAGGameplayTags::Get().InputTag_2);

	// 이미 점프중이라면 더블점프한다.
	if (isJump)
	{
		//mIsJump = false;
		LaunchCharacter(FVector(0.f, 0.f, 700.f), true, true);
		PlayMontage(FName("DoubleJump"));
	}
	else
	{
		Jump();
		SetActionState(EActionState2::EAS_Jump2, true);
	}
}


//-------------------------------
// Skill Functions
//-------------------------------

void AValkyrie::Skill1Key()
{
	mSkillState = ESkillState::ESS_Sprint;

	
	AValkyriePlayerState* state = GetPlayerState<AValkyriePlayerState>();
	UAGAttributeSet* attributeSet = Cast<UAGAttributeSet>(state->GetAttributeSet());
	attributeSet->SetmMp(attributeSet->GetmMp() - 10.f);

	mWeapon->SetCollisionOnOff(false);


	TSubclassOf<AAGSkillActor> skillActor = *mSkillmap.Find(EValkyrieSkill::EVS_Sprint);
	FActorSpawnParameters	params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AAGSkillActor* sk = GetWorld()->SpawnActor<AAGSkillActor>(skillActor, GetActorLocation(), GetActorRotation(), params);
	sk->SetOwnerActor(this);
	sk->FindTarget();

	mSkillActorMap.Add(EValkyrieSkill::EVS_Sprint, sk);

	PlayMontage(FName("Sprint"));
}

void AValkyrie::Skill2Key()
{
	//PrintViewport(10.f, FColor::Black, FString("Skill2Key"));

	mSkillState = ESkillState::ESS_Ribbon;

	FActorSpawnParameters	params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	TSubclassOf<AAGSkillActor> skillActor = *mSkillmap.Find(EValkyrieSkill::EVS_Range);
	AAGSkillActor* sk = GetWorld()->SpawnActor<AAGSkillActor>(skillActor, GetActorLocation(), GetActorRotation(), params);
	sk->SetOwnerActor(this);

	mSkillActorMap.Add(EValkyrieSkill::EVS_Range, sk);

	mWeapon->SetCollisionOnOff(false);
}

void AValkyrie::Skill2KeyPressing()
{
	AAGSkillActor* sk = *mSkillActorMap.Find(EValkyrieSkill::EVS_Range);
	Cast<AValkyrieRange>(sk)->SetIsPress(true);

}

void AValkyrie::Skill2KeyUp()
{
	//PrintViewport(1.f, FColor::Blue, FString("Released"));

	AAGSkillActor* sk = *mSkillActorMap.Find(EValkyrieSkill::EVS_Range);
	Cast<AValkyrieRange>(sk)->SetIsKeyReleased(true);

	if (Cast<AValkyrieRange>(sk)->GetIsPress())
	{
		UAnimMontage* montage = *mMontages.Find(FName("Ribbon"));
		mAnimInst->Montage_Resume(montage);
		mAnimInst->Montage_SetPlayRate(montage, 0.7f);

		Cast<AValkyrieRange>(sk)->SetIsPress(false);
	}

}

void AValkyrie::Skill3Key()
{
	mSkillState = ESkillState::ESS_Slash;
	
	AValkyriePlayerState* state = GetPlayerState<AValkyriePlayerState>();
	UAGAttributeSet* attributeSet = Cast<UAGAttributeSet>(state->GetAttributeSet());
	attributeSet->SetmMp(attributeSet->GetmMp() - 10.f);

	
	mWeapon->SetCollisionOnOff(false);
	
	bool isContainSkillActor = mSkillActorMap.Contains(EValkyrieSkill::EVS_Slash);
	if (!isContainSkillActor)
	{
		FActorSpawnParameters	params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		TSubclassOf<AAGSkillActor> skillActor = *mSkillmap.Find(EValkyrieSkill::EVS_Slash);
		AAGSkillActor* sk = GetWorld()->SpawnActor<AAGSkillActor>(skillActor, GetActorLocation(), GetActorRotation(), params);
		sk->SetOwnerActor(this);
		Cast<AValkyrieContinuousSlash>(sk)->InputPressed();

		mSkillActorMap.Add(EValkyrieSkill::EVS_Slash, sk);
	}
	else
	{
		AAGSkillActor* skillActor = *mSkillActorMap.Find(EValkyrieSkill::EVS_Slash);
		Cast<AValkyrieContinuousSlash>(skillActor)->InputPressed();
	}

	//PrintViewport(1.f, FColor::Yellow, FString::Printf(TEXT("index : %d"), mSlashSkillIndex));
}

void AValkyrie::Skill4Key()
{
	AValkyriePlayerState* state = GetPlayerState<AValkyriePlayerState>();
	UAGAttributeSet* attributeSet = Cast<UAGAttributeSet>(state->GetAttributeSet());
	attributeSet->SetmMp(attributeSet->GetmMp() - 10.f);



	TSubclassOf<AAGSkillActor> skillActor = *mSkillmap.Find(EValkyrieSkill::EVS_FallingSword);
	FActorSpawnParameters	params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AAGSkillActor* sk = GetWorld()->SpawnActor<AAGSkillActor>(skillActor, GetActorLocation(), GetActorRotation(), params);
	sk->SetOwnerActor(this);
	sk->Activate();
	mSkillActorMap.Add(EValkyrieSkill::EVS_FallingSword, sk);

	////////////

	mSkillState = ESkillState::ESS_HardAttack;
	
}


//-------------------------------
// Combat Functions
//-------------------------------

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

//-------------------------------
// Animation Montage Functions
//-------------------------------

void AValkyrie::PlayMontage(FName _montageName, FName _sectionName)
{
	UAnimMontage* montage = *mMontages.Find(_montageName);
	mAnimInst->Montage_Play(montage);
	if (_sectionName != "")
		mAnimInst->Montage_JumpToSection(_sectionName, montage);
}

void AValkyrie::SetMontagePlayRate(FName MontageName, float PlayRate)
{
	UAnimMontage* montage = *mMontages.Find(MontageName);
	mAnimInst->Montage_SetPlayRate(montage, PlayRate);

	//if (mSkillState == ESkillState::ESS_Ribbon)
	//{
	//	montage = *mMontages.Find(FName("Ribbon"));
	//	mAnimInst->Montage_SetPlayRate(montage, 0.1f);
	//	
	//}

	//switch (mSkillState)
	//{
	//case ESkillState::ESS_Sprint:
	//	montage = *mMontages.Find(FName("Sprint"));
	//	mAnimInst->Montage_SetPlayRate(montage, 0.1f);
	//	break;
	//case ESkillState::ESS_Ribbon:
	//	montage = *mMontages.Find(FName("Ribbon"));
	//	mAnimInst->Montage_SetPlayRate(montage, 0.1f);
	//	break;
	//}
}

//-------------------------------
// Fresnel Functions
//-------------------------------

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

//-------------------------------
// Timeline/Curve Functions
//-------------------------------

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

//-------------------------------
// Combat Effect Functions
//-------------------------------

void AValkyrie::SpawnEffect()
{
	FActorSpawnParameters	SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector location = FVector();

	
	if (CheckActionState(EActionState2::EAS_NormalAttack, false))
	{
		AAGSkillActor* sk = *mSkillActorMap.Find(EValkyrieSkill::EVS_NormalAttack);
		sk->SpawnEffect();
	}

	

	switch (mSkillState)
	{
	case ESkillState::ESS_Sprint:
	{
		AAGSkillActor* sk = *mSkillActorMap.Find(EValkyrieSkill::EVS_Sprint);
		sk->SpawnEffect();
	}
		break;

	case ESkillState::ESS_Ribbon:
	{
		AAGSkillActor* sk = *mSkillActorMap.Find(EValkyrieSkill::EVS_Range);
		Cast<AValkyrieRange>(sk)->SetIsPress(false);
		sk->SpawnEffect();
	}
	break;

	case ESkillState::ESS_Slash:
	{
		AAGSkillActor* sk = *mSkillActorMap.Find(EValkyrieSkill::EVS_Slash);
		sk->SpawnEffect();
	}
	break;

	case ESkillState::ESS_HardAttack:
	{
		AAGSkillActor* sk = *mSkillActorMap.Find(EValkyrieSkill::EVS_FallingSword);
		sk->SpawnEffect();
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

void AValkyrie::SetMotionWarpingComponent(const FVector& TargetLocation)
{
	FMotionWarpingTarget mwt;
	mwt.Transform.SetLocation(TargetLocation);
	mwt.Transform.SetRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation).Quaternion());
	mMotionWarpComp->AddOrUpdateWarpTarget(FName("SprintTarget"), mwt);
}

void AValkyrie::SetMp(float NewValue)
{
	AValkyriePlayerState* state = GetPlayerState<AValkyriePlayerState>();
	UAGAttributeSet* attributeSet = Cast<UAGAttributeSet>(state->GetAttributeSet());
	attributeSet->SetmMp(attributeSet->GetmMp() + NewValue);
}

void AValkyrie::SetAnimDelegate()
{
	mAnimInst->mOnAttackEnd.AddLambda([this]()->void {
		AAGSkillActor* sk = *mSkillActorMap.Find(EValkyrieSkill::EVS_NormalAttack);
		Cast<AValkyrieNormalAttack>(sk)->Notify_AttackEnd();
		mSkillActorMap.Remove(EValkyrieSkill::EVS_NormalAttack);

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
		AAGSkillActor* sk = *mSkillActorMap.Find(EValkyrieSkill::EVS_NormalAttack);
		Cast<AValkyrieNormalAttack>(sk)->Notify_NextAttackCheck();
		});

	mAnimInst->mOnLaunch.AddLambda([this]() -> void {
		GetCharacterMovement()->BrakingFrictionFactor = 0.f;
	});

	mAnimInst->mSkillEnd.AddLambda([this]() -> void {
		if (mSkillState == ESkillState::ESS_Sprint)
		{
			GetCharacterMovement()->BrakingFrictionFactor = 2.f;
			AAGSkillActor* sk = *mSkillActorMap.Find(EValkyrieSkill::EVS_Sprint);
			sk->SkillEnd();
			mSkillActorMap.Remove(EValkyrieSkill::EVS_Sprint);

		}
		else if (mSkillState == ESkillState::ESS_Ribbon)
		{
			AAGSkillActor* sk = *mSkillActorMap.Find(EValkyrieSkill::EVS_Range);
			sk->SkillEnd();
			mSkillActorMap.Remove(EValkyrieSkill::EVS_Range);
		}
		else if (mSkillState == ESkillState::ESS_Slash)
		{
			AAGSkillActor* sk = *mSkillActorMap.Find(EValkyrieSkill::EVS_Slash);
			sk->SkillEnd();
			mSkillActorMap.Remove(EValkyrieSkill::EVS_Slash);
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
		SetActionState(EActionState2::EAS_Jump2, false);
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
		AAGSkillActor* sk = *mSkillActorMap.Find(EValkyrieSkill::EVS_Slash);
		Cast<AValkyrieContinuousSlash>(sk)->Notify_SlashEnable();
		});

	mAnimInst->mOnPause.AddLambda([this]()-> void {

		if (mSkillState == ESkillState::ESS_Ribbon)
		{
			AAGSkillActor* sk = *mSkillActorMap.Find(EValkyrieSkill::EVS_Range);
			if (AValkyrieRange* rsk = Cast<AValkyrieRange>(sk))
			{
				//float f = rsk->GetPressingTime();
				//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("%f"), f));
				bool isEnd = rsk->GetIsKeyReleased();
				if (isEnd) return;

				if (!isEnd)
				{
					//PrintViewport(1.f, FColor::Blue, FString("Pause"));
					UAnimMontage* montage = *mMontages.Find(FName("Ribbon"));
					mAnimInst->Montage_Pause(montage);
				}
				else
				{
					UAnimMontage* montage = *mMontages.Find(FName("Ribbon"));
					mAnimInst->Montage_SetPlayRate(montage, 0.5f);
				}
			}
			//if (mSkill2PressingTime <= 0.f)
			//{
			//	UAnimMontage* montage = *mMontages.Find(FName("Ribbon"));
			//	//mAnimInst->Montage_Resume(montage);
			//	mAnimInst->Montage_Pause(montage);
			//}
		}
		});
}

void AValkyrie::PrintAllActionState()
{
	TArray<bool> check;
	TArray<FString> checkStr;

	check.Add(mStateType & (1 << static_cast<uint8>(EActionState2::EAS_Idle2)));
	checkStr.Add("Idle");

	check.Add(mStateType & (1 << static_cast<uint8>(EActionState2::EAS_Move2)));
	checkStr.Add("Move");

	check.Add(mStateType & (1 << static_cast<uint8>(EActionState2::EAS_Jump2)));
	checkStr.Add("Jump");

	check.Add(mStateType & (1 << static_cast<uint8>(EActionState2::EAS_Crouch2)));
	checkStr.Add("Crouch");

	check.Add(mStateType & (1 << static_cast<uint8>(EActionState2::EAS_Guard2)));
	checkStr.Add("Guard");

	int i = 0, cnt = check.Num();
	FString str = "";
	for (i = 0; i < cnt; ++i)
	{
		str += checkStr[i];
		if (check[i])
			str += " O\n";
		else
			str += " X\n";
	}
	PrintViewport(1.f, FColor::Blue, str);

}