// Fill out your copyright notice in the Description page of Project Settings.


#include "AGPlayer.h"
#include "ValkyrieAnimInstance.h"

AAGPlayer::AAGPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 450.f;

	mSpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	mSpringArmComp->SetupAttachment(GetRootComponent());
	mSpringArmComp->TargetArmLength = 300.f;
	mSpringArmComp->bUsePawnControlRotation = true;

	mCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	mCameraComp->SetupAttachment(mSpringArmComp);
	

	mIsAttacking = false;

	mCharacterState = ECharacterState::ECS_Unequipped;
	mActionState = EActionState::EAS_Idle;
}

void AAGPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AAGPlayer::MoveForward(float _value)
{
	if ((_value != 0.f) && Controller && mActionState != EActionState::EAS_Attack)
	{
		mActionState = EActionState::EAS_Move;
		const FRotator controlRotation = GetControlRotation();
		const FRotator yawRotation(0.f, controlRotation.Yaw, 0.f);
		const FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(direction, _value);
	}
}

void AAGPlayer::MoveHorizontal(float _value)
{
	if ((_value != 0.f) && Controller && mActionState != EActionState::EAS_Attack)
	{
		mActionState = EActionState::EAS_Move;
		const FRotator controlRotation = GetControlRotation();
		const FRotator yawRotation(0.f, controlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, _value);
	}
}

void AAGPlayer::Turn(float _value)
{
	AddControllerYawInput(_value);
}

void AAGPlayer::LookUp(float _value)
{
	AddControllerPitchInput(_value);
}

void AAGPlayer::JumpKey()
{
	Jump();
}

void AAGPlayer::EquipWeaponKey()
{
}

void AAGPlayer::NormalAttackKey()
{
}

void AAGPlayer::PlayMontage(FName _montageName, FName _sectionName)
{
	
}

void AAGPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAGPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis<AAGPlayer>(TEXT("MoveForward"), this, &AAGPlayer::MoveForward);
	PlayerInputComponent->BindAxis<AAGPlayer>(TEXT("MoveHorizontal"), this, &AAGPlayer::MoveHorizontal);
	PlayerInputComponent->BindAxis<AAGPlayer>(TEXT("LookUp"), this, &AAGPlayer::LookUp);
	PlayerInputComponent->BindAxis<AAGPlayer>(TEXT("Turn"), this, &AAGPlayer::Turn);

	PlayerInputComponent->BindAction<AAGPlayer>(TEXT("EquipWeapon"), EInputEvent::IE_Released,
		this, &AAGPlayer::EquipWeaponKey);
	PlayerInputComponent->BindAction<AAGPlayer>(TEXT("NormalAttack"), EInputEvent::IE_Pressed,
		this, &AAGPlayer::NormalAttackKey);
	PlayerInputComponent->BindAction<AAGPlayer>(TEXT("Jump"), EInputEvent::IE_Pressed,
		this, &AAGPlayer::JumpKey);
}


