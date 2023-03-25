// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//---------------------------
	// 컴포넌트 생성.
	//---------------------------
	mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

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
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("MoveForward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("MoveHorizontal"), this, &APlayerCharacter::MoveHorizontal);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("MouseRotateY"), this, &APlayerCharacter::MouseRotateY);
	PlayerInputComponent->BindAxis<APlayerCharacter>(TEXT("MouseRotateZ"), this, &APlayerCharacter::MouseRotateZ);
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

	AddControllerPitchInput(_scale * 180.f * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::MouseRotateZ(float _scale)
{
	if (_scale == 0.f)
		return;

	AddControllerYawInput(_scale * 180.f * GetWorld()->GetDeltaSeconds());
}

