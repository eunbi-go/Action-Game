// Fill out your copyright notice in the Description page of Project Settings.


#include "AGPlayerController.h"
#include "../Particle/Decal.h"
#include "../AbilitySystem/AGAbilitySystemLibrary.h"
#include "../Input/AGInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "../AbilitySystem/AGAbilitySystemComponent.h"


AAGPlayerController::AAGPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	bShowMouseCursor = true;

	static ConstructorHelpers::FObjectFinder<UAGInputConfig> input_config(TEXT("AGInputConfig'/Game/Blueprints/Valkyrie_BP/Input/DA_AGInputConfig.DA_AGInputConfig'"));
	if (input_config.Succeeded())
	{
		mInputConfig = input_config.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> input_mapping_context(TEXT("InputMappingContext'/Game/Blueprints/Valkyrie_BP/Input/IMC_AGContext.IMC_AGContext'"));
	if (input_mapping_context.Succeeded())
	{
		mInputMappingContext = input_mapping_context.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> input_action_W(TEXT("InputAction'/Game/Blueprints/Valkyrie_BP/Input/InputActions/IA_W.IA_W'"));
	if (input_action_W.Succeeded())
	{
		mInputAction_W = input_action_W.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> input_action_A(TEXT("InputAction'/Game/Blueprints/Valkyrie_BP/Input/InputActions/IA_A.IA_A'"));
	if (input_action_A.Succeeded())
	{
		mInputAction_A = input_action_A.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> input_action_S(TEXT("InputAction'/Game/Blueprints/Valkyrie_BP/Input/InputActions/IA_S.IA_S'"));
	if (input_action_S.Succeeded())
	{
		mInputAction_S = input_action_S.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> input_action_D(TEXT("InputAction'/Game/Blueprints/Valkyrie_BP/Input/InputActions/IA_D.IA_D'"));
	if (input_action_D.Succeeded())
	{
		mInputAction_D = input_action_D.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> input_action_Q(TEXT("InputAction'/Game/Blueprints/Valkyrie_BP/Input/InputActions/IA_Q.IA_Q'"));
	if (input_action_Q.Succeeded())
	{
		mInputAction_Q = input_action_Q.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> input_action_E(TEXT("InputAction'/Game/Blueprints/Valkyrie_BP/Input/InputActions/IA_E.IA_E'"));
	if (input_action_E.Succeeded())
	{
		mInputAction_E = input_action_E.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> input_action_R(TEXT("InputAction'/Game/Blueprints/Valkyrie_BP/Input/InputActions/IA_R.IA_R'"));
	if (input_action_R.Succeeded())
	{
		mInputAction_R = input_action_R.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> input_action_T(TEXT("InputAction'/Game/Blueprints/Valkyrie_BP/Input/InputActions/IA_T.IA_T'"));
	if (input_action_T.Succeeded())
	{
		mInputAction_T = input_action_T.Object;
	}
}

void AAGPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (APawn* controlledPawn = GetPawn())
	{


	}
}

void AAGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//FInputModeGameOnly	Mode;
	//FInputModeGameAndUI	Mode;
	//Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	//Mode.SetHideCursorDuringCapture(false);
	//SetInputMode(Mode);

	FInputModeGameAndUI	Mode;
	SetInputMode(Mode);


	bShowMouseCursor = true;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(mInputMappingContext, 0);
	}
}

void AAGPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AAGPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void AAGPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AAGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAGInputComponent* inputComp = CastChecked<UAGInputComponent>(InputComponent);
	inputComp->BindAbilityAction(mInputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAGPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void AAGPlayerController::SetInputModeType(INPUT_MODE_TYPE _type)
{
	/*switch (_type)
	{
	case INPUT_MODE_TYPE::GAME_ONLY:
	{
		FInputModeGameOnly	Mode;
		SetInputMode(Mode);
	}
	break;

	case INPUT_MODE_TYPE::GAME_UI:
	{
		FInputModeGameAndUI	Mode;
		SetInputMode(Mode);
	}
	break;
	}*/
}

UAGAbilitySystemComponent* AAGPlayerController::GetASC()
{
	if (mAGasc == nullptr)
	{
		mAGasc = Cast<UAGAbilitySystemComponent>(UAGAbilitySystemLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}

	return mAGasc;
}

void AAGPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	PrintViewport(3.f, FColor::Red, *InputTag.ToString());
}

void AAGPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() == nullptr)
		return;

	GetASC()->AbilityInputTagReleased(InputTag);
}

void AAGPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() == nullptr)
		return;

	GetASC()->AbilityInputTagHeld(InputTag);
}
