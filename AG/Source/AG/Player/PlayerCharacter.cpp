// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "PlayerAnimInstance.h"
#include "../Particle/ParticleCascade.h"
#include "../Basic/WeaponActor.h"
#include "../AGGameInstance.h"
#include "../Basic/WeaponActor.h"
#include "AGPlayerController.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//---------------------------
	// ������Ʈ ����.
	//---------------------------
	mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mDash = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DashEffect"));

	mDash->SetupAttachment(GetMesh());
	mSpringArm->SetupAttachment(GetMesh());
	mCamera->SetupAttachment(mSpringArm);



	//---------------------------
	// 3��Ī ī�޶� ����.
	//---------------------------
	mSpringArm->TargetArmLength = 500.f;
	mSpringArm->SetRelativeLocation(FVector(0.0, 0.0, 160.0));
	mSpringArm->SetRelativeRotation(FRotator::ZeroRotator);
	mSpringArm->bUsePawnControlRotation = true;
	mSpringArm->bInheritPitch = true;
	mSpringArm->bInheritRoll = true;
	mSpringArm->bInheritYaw = true;
	mSpringArm->bDoCollisionTest = true;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);




	//---------------------------
	// �浹 ����.
	//---------------------------
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);



	//---------------------------
	// Jump.
	//---------------------------
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	JumpMaxCount = 2;
	JumpMaxHoldTime = 0.4f;

	

	//---------------------------
	// Camera Lag.
	//---------------------------
	mSpringArm->CameraLagSpeed = 8.f;
	mSpringArm->CameraRotationLagSpeed = 8.f;
	mSpringArm->bEnableCameraLag = false;



	//---------------------------
	// Particles, Actors ...
	//---------------------------
	mWeapon = nullptr;



	//-----------------------------------------
	// CameraShake ����.
	//-----------------------------------------
	static ConstructorHelpers::FClassFinder<UCameraShakeBase>	cameraShake(TEXT("Blueprint'/Game/Blueprints/CameraShake/CS_Gauge.CS_Gauge_C'"));

	if (cameraShake.Succeeded())
		mGaugeShake = cameraShake.Class;


	mIsEquipWeapon = false;




	mPlayerTableRowName = TEXT("Player");
	mIsDelay = false;
	mTeleportGauge = 0.f;
	mTargetPosition = FVector(0.f, 0.f, 0.f);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Camera View Pitch ���� ����.
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewPitchMin = -40.f;
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewPitchMax = 20.f;




	//---------------------------
	// Dash Niagara.
	//---------------------------
	UNiagaraSystem* Particle = LoadObject<UNiagaraSystem>(
		nullptr, TEXT("NiagaraSystem'/Game/Assets/Niagara/NS_Dash2.NS_Dash2'"));

	if (IsValid(Particle))
		mDash->SetAsset(Particle);
	mDash->SetVisibility(false);

	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;




	//---------------------------
	// Data Table �� Ȱ���� PlayerInfo ����.
	//---------------------------
	UAGGameInstance* gameInst =
		GetWorld()->GetGameInstance<UAGGameInstance>();

	const FPlayerTableInfo* info = gameInst->FindPlayerTable(mPlayerTableRowName);

	if (info)
	{
		mInfo.name = info->name;
		mInfo.attackPoint = info->attackPoint;
		mInfo.defensePoint = info->defensePoint;
		mInfo.hp = info->hp;
		mInfo.maxHp = info->maxHp;
		mInfo.mp = info->mp;
		mInfo.maxMp = info->maxMp;
		mInfo.level = info->level;
		mInfo.exp = info->exp;
		mInfo.gold = info->gold;
		mInfo.movingWalkSpeed = info->movingWalkSpeed;
		mInfo.movingRunSpeed = info->movingRunSpeed;
		mInfo.movingDashSpeed = info->movingDashSpeed;
		mInfo.attackDistance = info->attackDistance;

		GetCharacterMovement()->MaxWalkSpeed = mInfo.movingWalkSpeed;

		GetMesh()->SetSkeletalMesh(info->mesh);
		GetMesh()->SetAnimInstanceClass(info->playerAnimClass);
	}

	mAnimInst = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	//---------------------------
	// �����߿��� ��鸮�� �ʰ� �÷��̾�� ī�޶��� ���麤�Ͱ� ��ġ�ϵ��� ����.
	//---------------------------
	if (mAnimInst->GetPlayerMotion() == PLAYER_MOTION::NORMAL_ATTACK || mAnimInst->GetPlayerMotion() == PLAYER_MOTION::SKILL)
	{
		FVector cameraForwardVector = mCamera->GetForwardVector().GetSafeNormal2D();
		FRotator targetRot = FRotationMatrix::MakeFromX(cameraForwardVector).Rotator();
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), targetRot, DeltaTime, 10.0f));
	}



	//---------------------------
	// �÷��̾�� ���� ���� Ʈ���̽��� ������� �Ÿ��� ���� ���� �Ÿ� ���ϰ� �Ǹ� ���� ��� ���.
	//---------------------------
	if (mAnimInst->GetPlayerMotion() == PLAYER_MOTION::JUMP)
	{
		FVector startPos = GetActorLocation();
		FVector endPos = FVector(startPos.X, startPos.Y, -100.f);
		FHitResult hitInfo;
		FCollisionQueryParams params;
		bool isHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);

		if (isHit)
		{
			if (hitInfo.Distance <= 70.f)
			{
				PrintViewport(0.5f, FColor::Red, FString::Printf(TEXT("down")));
				mAnimInst->SetIsLandStart(true);
			}
		}
	}


	//PrintViewport(0.5f, FColor::Red, FString::Printf(TEXT("x: %f, y: %f, z: %f"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z));
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	

	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("MoveForward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("MoveHorizontal"), this, &APlayerCharacter::MoveHorizontal);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("MouseRotateY"), this, &APlayerCharacter::MouseRotateY);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("MouseRotateZ"), this, &APlayerCharacter::MouseRotateZ);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("Dash"), this, &APlayerCharacter::Dash);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("Gauge"), this, &APlayerCharacter::Gauge);


	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("ChangePlayMode"), EInputEvent::IE_Pressed,
		this, &APlayerCharacter::ChangePlayModeKey);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("Evade"), EInputEvent::IE_Pressed,
		this, &APlayerCharacter::EvadeKey);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("EvadeBackward"), EInputEvent::IE_Pressed,
		this, &APlayerCharacter::EvadeBackwardKey);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("Jump"), EInputEvent::IE_Pressed,
		this, &APlayerCharacter::JumpKey);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("Jump"), EInputEvent::IE_Released,
		this, &APlayerCharacter::JumpEnd);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("EquipWeapon"), EInputEvent::IE_Released,
		this, &APlayerCharacter::EquipWeaponKey);
	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("NormalAttack"), EInputEvent::IE_Pressed,
		this, &APlayerCharacter::NormalAttackKey);

	PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("Skill1"), EInputEvent::IE_Pressed,
		this, &APlayerCharacter::Skill1Key);
	/*PlayerInputComponent->BindAction<APlayerCharacter>(TEXT("GaugeEnd"), EInputEvent::IE_Released,
		this, &APlayerCharacter::GaugeEnd);*/


	
}

void APlayerCharacter::NormalAttackCheck()
{
}

void APlayerCharacter::Skill1()
{
}

void APlayerCharacter::Skill2()
{
}

void APlayerCharacter::UseSkill(SKILL_TYPE _skillType)
{
}

void APlayerCharacter::SpawnSkill(SKILL_TYPE _skillType, int32 _skillInfoArrayIndex)
{
	SpawnSkill(_skillType, _skillInfoArrayIndex);
}

void APlayerCharacter::EquipWeaponToHand()
{
	FActorSpawnParameters	SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FRotator rot = FRotator(90.0f, GetActorRotation().Yaw, GetActorRotation().Roll);
	AParticleCascade* Particle =
		GetWorld()->SpawnActor<AParticleCascade>(
			GetActorLocation(),
			GetActorRotation(),
			SpawnParam);

	Particle->SetParticle(TEXT("ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Ability/Defense/P_Shield_Spawn.P_Shield_Spawn'"));


	mWeapon->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::KeepRelativeTransform,
		TEXT("middle_01_r"));

	mWeapon->SetActorRelativeLocation(FVector(-1.0f, 9.0f, -1.0f));
	mWeapon->SetActorRelativeRotation(FRotator(9.f, 151.f, 27.0f));
}

void APlayerCharacter::SetWeaponTrailOnOff(bool _value)
{
	mWeapon->SetTrailOnOff(_value);
}

void APlayerCharacter::ClickDestination()
{
	AAGPlayerController* playerController = Cast<AAGPlayerController>(GetController());

	if (IsValid(playerController))
		playerController->SpawnDecalOnMousePick();
}

void APlayerCharacter::MoveForward(float _scale)
{
	if (_scale == 0.f)
		return;

	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), _scale);
}

void APlayerCharacter::MoveHorizontal(float _scale)
{
	if (_scale == 0.f)
		return;

	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), _scale);
}

void APlayerCharacter::MouseRotateY(float _scale)
{
	if (_scale == 0.f)
		return;

	AddControllerPitchInput(_scale * 90.f * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::MouseRotateZ(float _scale)
{
	if (_scale == 0.f)
		return;

	AddControllerYawInput(_scale * 90.f * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::Dash(float _scale)
{
	if (_scale == 0.f)
	{
		CustomTimeDilation = 1.f;
		mAnimInst->FinishDash();

		// ī�޶� ���� ��.
		mSpringArm->bEnableCameraLag = false;
		mSpringArm->bEnableCameraRotationLag = false;

		mDash->SetVisibility(false);

		// ���� �ӵ��� ���ư���.
		if (mAnimInst->GetPlayerMotion() == PLAYER_MOTION::MOVE)
		{
			if (mAnimInst->GetIsRun())
				SetRunStateSpeed();
			else
				SetWalkStsteSpeed();
		}

		return;
	}
	


	if (mAnimInst->GetPlayerMotion() == PLAYER_MOTION::MOVE)
	{
		//CustomTimeDilation = 2.f;
		
		// ī�޶� �������� �÷��̾� ���� ȿ�� �ֱ�.
		mSpringArm->bEnableCameraLag = true;
		mSpringArm->bEnableCameraRotationLag = true;
		mAnimInst->Dash();
		mDash->SetVisibility(true);
	}
}

void APlayerCharacter::Gauge(float _scale)
{
	Gauge(_scale);
}

void APlayerCharacter::ChangePlayModeKey()
{
	//mAnimInst->ChangePlayMode();
}

void APlayerCharacter::EvadeKey()
{
	mAnimInst->Evade(DIRECTION::FORWARD);
}

void APlayerCharacter::EvadeBackwardKey()
{
	mAnimInst->Evade(DIRECTION::BACKWARD);
}

void APlayerCharacter::JumpKey()
{
	if (mAnimInst->GetPlayerMotion() == PLAYER_MOTION::IDLE || mAnimInst->GetPlayerMotion() == PLAYER_MOTION::MOVE)
	{
		Jump();
		bPressedJump = true;

		PrintViewport(0.5f, FColor::Red, FString::Printf(TEXT("JUMP")));
		mAnimInst->JumpStart();
	}
}

void APlayerCharacter::JumpEnd()
{
	bPressedJump = false;
}

void APlayerCharacter::EquipWeaponKey()
{
	if (mIsEquipWeapon)
		return;

	mAnimInst->EquipWeapon();
	mIsEquipWeapon = true;
}

void APlayerCharacter::NormalAttackKey()
{
	if (!mIsEquipWeapon)
		return;


	AAGPlayerController* playerController = Cast<AAGPlayerController>(GetController());

	if (IsValid(playerController) && mAnimInst->GetCurSkillType() == SKILL_TYPE::TELEPORT)
	{
		playerController->SpawnDecalOnMousePick();
		GaugeEnd();
	}
	else
	{
		// �Ϲ� ���� ��Ÿ�� ��� ����.
		mAnimInst->NormalAttack();
		mWeapon->SetTrailOnOff(true);
	}
}	

void APlayerCharacter::Skill1Key()
{
	Skill1();
}

void APlayerCharacter::GaugeEnd()
{
	if (mAnimInst->GetPlayerMotion() != PLAYER_MOTION::SKILL)
		return;

	//GaugeEnd();
}

void APlayerCharacter::StopLaunchCharacter()
{
	if (mAnimInst->GetPlayerMotion() != PLAYER_MOTION::SKILL)
		return;

	StopLaunchCharacter();
}

void APlayerCharacter::RestartSkill()
{
	if (mAnimInst->GetPlayerMotion() != PLAYER_MOTION::SKILL)
		return;

	RestartSkill();
}
