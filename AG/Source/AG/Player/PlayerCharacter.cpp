// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "PlayerAnimInstance.h"
#include "../Particle/ParticleCascade.h"
#include "../Basic/WeaponActor.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//---------------------------
	// 컴포넌트 생성.
	//---------------------------
	mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mDash = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DashEffect"));

	mDash->SetupAttachment(GetMesh());
	mSpringArm->SetupAttachment(GetMesh());
	mCamera->SetupAttachment(mSpringArm);



	//---------------------------
	// 3인칭 카메라 세팅.
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
	// 충돌 세팅.
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




	mIsEquipWeapon = false;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	mAnimInst = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());


	// Camera View Pitch 각도 제한.
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewPitchMin = -40.f;
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->ViewPitchMax = 20.f;



	//---------------------------
	// Dash Niagara.
	//---------------------------
	UNiagaraSystem* Particle = LoadObject<UNiagaraSystem>(
		nullptr, TEXT("NiagaraSystem'/Game/Assets/Niagara/NS_Dash.NS_Dash'"));

	if (IsValid(Particle))
		mDash->SetAsset(Particle);
	mDash->SetVisibility(false);

	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
}

void APlayerCharacter::EquipWeaponToHand()
{
	mWeapon->AttachToComponent(GetMesh(),
		FAttachmentTransformRules::KeepRelativeTransform,
		TEXT("middle_01_r"));

	mWeapon->SetActorRelativeLocation(FVector(-1.0f, 9.0f, -1.0f));
	mWeapon->SetActorRelativeRotation(FRotator(9.f, 151.f, 27.0f));
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

		// 카메라 지연 끝.
		mSpringArm->bEnableCameraLag = false;
		mSpringArm->bEnableCameraRotationLag = false;

		mDash->SetVisibility(false);

		// 이전 속도로 돌아간다.
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
		
		// 카메라 지연으로 플레이어 가속 효과 주기.
		mSpringArm->bEnableCameraLag = true;
		mSpringArm->bEnableCameraRotationLag = true;
		mAnimInst->Dash();
		mDash->SetVisibility(true);
	}
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
}
