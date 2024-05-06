// Fill out your copyright notice in the Description page of Project Settings.


#include "AGPlayer.h"
#include "ValkyrieAnimInstance.h"
#include "CharacterStatComponent.h"
#include "../AGGameModeBase.h"
#include "../Widget/MainWidget.h"
#include "../Manager/InventoryManager.h"
#include "../Widget/InventoryWidget.h"
#include "../Widget/ItemQuickSlot.h"
#include "../AGSaveGame.h"
#include "../AGGameInstance.h"
#include "../Manager/InventoryManager.h"
#include "../Widget/HUD/AGHUD.h"
#include "ValkyriePlayerState.h"
#include "../AbilitySystem/AGAttributeSet.h"
#include "../Widget/ItemData.h"
#include "../AbilitySystem/AGAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

AAGPlayer::AAGPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 450.f;

	//mSpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	mSpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	mSpringArmComp->SetupAttachment(GetRootComponent());
	mSpringArmComp->TargetArmLength = 500.f;
	mSpringArmComp->bUsePawnControlRotation = true;
	

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	
	// 데미지 수신 가능.
	SetCanBeDamaged(true);


	mCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	//TPPCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TPPCamera"));
	mCameraComp->SetupAttachment(mSpringArmComp);
	
	mDash = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DashEffect"));
	mDash->SetupAttachment(GetMesh());
	mDash->SetRelativeLocation(FVector(0.f, 0.f, 90.f));
	mDash->SetRelativeRotation(FRotator(0.f, 270.f, 0.f));
	//mStat = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("Stat"));

	mIsAttacking = false;

	mCharacterState = ECharacterState::ECS_Unequipped;
	mSkillState = ESkillState::ESS_None;

	mPlayerTableRowName = TEXT("Player");
}

void AAGPlayer::BeginPlay()
{
	Super::BeginPlay();

	UInventoryManager::GetInst(GetWorld())->InventoryOnOff(false);
	//GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;


	//---------------------------
	// Data Table 을 활용해 PlayerInfo 세팅.
	//---------------------------
	UAGGameInstance* gameInst = GetWorld()->GetGameInstance<UAGGameInstance>();

	FString FullPath = FPaths::ProjectSavedDir() + TEXT("SaveGames/Save.txt");

	TSharedPtr<FArchive>	Reader = MakeShareable(IFileManager::Get().CreateFileReader(*FullPath));
	const FPlayerTableInfo* info = gameInst->FindPlayerTable(mPlayerTableRowName);

	//if (Reader.IsValid())
	//{
	//	*Reader.Get() << mStat->GetInfo().name;
	//	*Reader.Get() << mStat->GetInfo().attackPoint;
	//	*Reader.Get() << mStat->GetInfo().defensePoint;
	//	*Reader.Get() << mStat->GetInfo().hp;
	//	*Reader.Get() << mStat->GetInfo().maxHp;
	//	*Reader.Get() << mStat->GetInfo().mp;
	//	*Reader.Get() << mStat->GetInfo().maxMp;
	//	*Reader.Get() << mStat->GetInfo().level;
	//	*Reader.Get() << mStat->GetInfo().exp;
	//	*Reader.Get() << mStat->GetInfo().gold;
	//	*Reader.Get() << mStat->GetInfo().movingWalkSpeed;
	//	*Reader.Get() << mStat->GetInfo().movingRunSpeed;
	//	*Reader.Get() << mStat->GetInfo().movingDashSpeed;
	//	*Reader.Get() << mStat->GetInfo().attackDistance;

	//	mStat->SetHp(mStat->GetInfo().hp);
	//	mStat->SetMp(mStat->GetInfo().mp);
	//	mStat->SetCoin(mStat->GetInfo().gold);
	//	
	//}
	//else
	//{

	//	if (info)
	//	{
	//		mStat->GetInfo().name = info->name;
	//		mStat->GetInfo().attackPoint = info->attackPoint;
	//		mStat->GetInfo().defensePoint = info->defensePoint;
	//		mStat->GetInfo().hp = info->hp;
	//		mStat->GetInfo().maxHp = info->maxHp;
	//		mStat->GetInfo().mp = info->mp;
	//		mStat->GetInfo().maxMp = info->maxMp;
	//		mStat->GetInfo().level = info->level;
	//		mStat->GetInfo().exp = info->exp;
	//		mStat->GetInfo().gold = info->gold;
	//		mStat->GetInfo().movingWalkSpeed = info->movingWalkSpeed;
	//		mStat->GetInfo().movingRunSpeed = info->movingRunSpeed;
	//		mStat->GetInfo().movingDashSpeed = info->movingDashSpeed;
	//		mStat->GetInfo().attackDistance = info->attackDistance;
	//	}
	//}
	//GetCharacterMovement()->MaxWalkSpeed = mStat->GetInfo().movingRunSpeed;
	//GetCharacterMovement()->MaxWalkSpeedCrouched = 100.f;
	//AAGGameModeBase* gameMode = Cast<AAGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	//UMainWidget* mainWidget = gameMode->GetMainWidget();
	//mainWidget->SetCharacterStat(mStat);
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;



	//---------------------------
	// Dash Niagara.
	//---------------------------
	UNiagaraSystem* Particle = LoadObject<UNiagaraSystem>(
		nullptr, TEXT("NiagaraSystem'/Game/BlinkAndDashVFX/VFX_Niagara/NS_Dash_Fire.NS_Dash_Fire'"));
	if (IsValid(Particle))
		mDash->SetAsset(Particle);
	mDash->SetActive(false);
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
	PlayerInputComponent->BindAction<AAGPlayer>(TEXT("NormalAttack"), EInputEvent::IE_Released,
		this, &AAGPlayer::NormalAttackKeyReleased);
	PlayerInputComponent->BindAction<AAGPlayer>(TEXT("Jump"), EInputEvent::IE_Pressed,
		this, &AAGPlayer::JumpKey);
	PlayerInputComponent->BindAction<AAGPlayer>(TEXT("Crouch"), EInputEvent::IE_Pressed,
		this, &AAGPlayer::CrouchKey);
	PlayerInputComponent->BindAction<AAGPlayer>(TEXT("Dash"), EInputEvent::IE_Pressed,
		this, &AAGPlayer::DashKey);
	PlayerInputComponent->BindAction<AAGPlayer>(TEXT("Guard"), EInputEvent::IE_Pressed,
		this, &AAGPlayer::GuardKey);
	PlayerInputComponent->BindAction<AAGPlayer>(TEXT("CheatMp"), EInputEvent::IE_Pressed,
		this, &AAGPlayer::CheatMpKey);

	/**
	 * Item
	 */
	PlayerInputComponent->BindAction<AAGPlayer>(TEXT("InventoryOnOff"), EInputEvent::IE_Pressed,
		this, &AAGPlayer::InventoryOnOffKey);
	PlayerInputComponent->BindAction<AAGPlayer>(TEXT("Item1"), EInputEvent::IE_Pressed,
		this, &AAGPlayer::Item1Key);
	PlayerInputComponent->BindAction<AAGPlayer>(TEXT("Item2"), EInputEvent::IE_Pressed,
		this, &AAGPlayer::Item2Key);
	PlayerInputComponent->BindAction<AAGPlayer>(TEXT("Item3"), EInputEvent::IE_Pressed,
		this, &AAGPlayer::Item3Key);
	PlayerInputComponent->BindAction<AAGPlayer>(TEXT("Item4"), EInputEvent::IE_Pressed,
		this, &AAGPlayer::Item4Key);
	PlayerInputComponent->BindAction<AAGPlayer>(TEXT("Item5"), EInputEvent::IE_Pressed,
		this, &AAGPlayer::Item5Key);

}

void AAGPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (mHorizontalInputValue == 0.f && mForwardInputValue == 0.f)
	{
		SetActionState(EActionState::EAS_Idle, true);
		SetActionState(EActionState::EAS_Move, false);
	}
	else
	{
		SetActionState(EActionState::EAS_Idle, false);
		SetActionState(EActionState::EAS_Move, true);
	}
}

void AAGPlayer::MoveForward(float _value)
{
	mForwardInputValue = _value;
	

	if ((_value != 0.f) && Controller && !CheckActionState(EActionState::EAS_Attack_Skill, false))
	{
		const FRotator controlRotation = GetControlRotation();
		const FRotator yawRotation(0.f, controlRotation.Yaw, 0.f);
		const FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(direction, _value);
	}
}

void AAGPlayer::MoveHorizontal(float _value)
{
	mHorizontalInputValue = _value;

	if ((_value != 0.f) && Controller && !CheckActionState(EActionState::EAS_Attack_Skill, false))
	{
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

void AAGPlayer::CrouchKey()
{
}

void AAGPlayer::GuardKey()
{
	
}

void AAGPlayer::EquipWeaponKey()
{
}

void AAGPlayer::NormalAttackKey()
{
}

void AAGPlayer::NormalAttackKeyReleased()
{
}

void AAGPlayer::InventoryOnOffKey()
{
	if (UInventoryManager::GetInst(GetWorld())->IsInventoryOn())
	{
		UInventoryManager::GetInst(GetWorld())->InventoryOnOff(false);
	}
	else
		UInventoryManager::GetInst(GetWorld())->InventoryOnOff(true);
}

void AAGPlayer::Item1Key()
{
	AAGGameModeBase* gameMode = Cast<AAGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	check(gameMode);

	AAGHUD* hud = Cast<AAGHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (!IsValid(hud))
		return;
	UMainWidget* mainWidget = hud->mMainWidget;
	if (!IsValid(mainWidget))
		return;

	mainWidget->GetItemQuickSlot()->UseItem(1, this);
}

void AAGPlayer::Item2Key()
{
	AAGGameModeBase* gameMode = Cast<AAGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	check(gameMode);

	AAGHUD* hud = Cast<AAGHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (!IsValid(hud))
		return;
	UMainWidget* mainWidget = hud->mMainWidget;
	if (!IsValid(mainWidget))
		return;

	mainWidget->GetItemQuickSlot()->UseItem(2, this);
}

void AAGPlayer::Item3Key()
{
	AAGGameModeBase* gameMode = Cast<AAGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	check(gameMode);

	AAGHUD* hud = Cast<AAGHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (!IsValid(hud))
		return;
	UMainWidget* mainWidget = hud->mMainWidget;
	if (!IsValid(mainWidget))
		return;

	mainWidget->GetItemQuickSlot()->UseItem(3, this);
}

void AAGPlayer::Item4Key()
{
	AAGGameModeBase* gameMode = Cast<AAGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	check(gameMode);

	AAGHUD* hud = Cast<AAGHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (!IsValid(hud))
		return;
	UMainWidget* mainWidget = hud->mMainWidget;
	if (!IsValid(mainWidget))
		return;

	mainWidget->GetItemQuickSlot()->UseItem(4, this);
}

void AAGPlayer::Item5Key()
{
	AAGGameModeBase* gameMode = Cast<AAGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	check(gameMode);

	AAGHUD* hud = Cast<AAGHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (!IsValid(hud))
		return;
	UMainWidget* mainWidget = hud->mMainWidget;
	if (!IsValid(mainWidget))
		return;

	mainWidget->GetItemQuickSlot()->UseItem(5, this);
}

void AAGPlayer::DashKey()
{
	//mDash->SetVisibility(true);
	mDash->SetActive(true);

	FLatentActionInfo Info;
	Info.CallbackTarget = this;
	UKismetSystemLibrary::MoveComponentTo(
		GetCapsuleComponent(),
		GetActorLocation() + GetActorForwardVector() * 1000.f,
		GetActorForwardVector().Rotation(),
		true,
		true,
		0.35f,
		false,
		EMoveComponentAction::Type::Move,
		Info);
}

void AAGPlayer::CheatMpKey()
{
	AValkyriePlayerState* state = GetPlayerState<AValkyriePlayerState>();
	UAGAttributeSet* attributeSet = Cast<UAGAttributeSet>(state->GetAttributeSet());
	attributeSet->SetmMaxMp(attributeSet->GetmMaxMp() + 100.f);
}

void AAGPlayer::PlayMontage(FName _montageName, FName _sectionName)
{
	
}

void AAGPlayer::SpawnEffect()
{
}

float AAGPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	int32 damage = (int32)Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	AValkyriePlayerState* state = GetPlayerState<AValkyriePlayerState>();
	UAGAttributeSet* attributeSet = Cast<UAGAttributeSet>(state->GetAttributeSet());

	damage -= attributeSet->GetmIntelligence();
	if (damage < 1)
		damage = 1;

	// Guard 상태일 경우, 데미지 피해량 감소
	bool isGuard = CheckActionState(EActionState::EAS_Guard, false);
	if (isGuard)
		damage /= 2;

	int hp = attributeSet->GetmHp();

	// death
	if (hp - damage <= 0)
	{

	}
	// hit
	else
	{
		attributeSet->SetmHp(hp - damage);
	}

	return 0.0f;
}

bool AAGPlayer::AddItem(EITEM_ID _itemID)
{
	AValkyriePlayerState* state = GetPlayerState<AValkyriePlayerState>();
	UAGAttributeSet* attributeSet = Cast<UAGAttributeSet>(state->GetAttributeSet());
	UAGGameInstance* gameInst = Cast<UAGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (_itemID == EITEM_ID::COIN)
	{
		const FItemAsset* item = gameInst->FindItemAssetTable("Coin");
		// 아이템 효과 적용

		UAbilitySystemComponent* targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(this);
		if (targetASC == nullptr)
			return false;

		FGameplayEffectContextHandle ecHandle = targetASC->MakeEffectContext();
		//ecHandle.AddSourceObject(this);

		const FGameplayEffectSpecHandle esHandle = targetASC->MakeOutgoingSpec(*item->asset, 1.f, ecHandle);
		const FActiveGameplayEffectHandle agpeHandle = targetASC->ApplyGameplayEffectSpecToSelf(*esHandle.Data.Get());

		//attributeSet->SetmCoin(attributeSet->GetmCoin() + 10);
		return true;
	}
	if (_itemID == EITEM_ID::END)
		return false;

	AAGGameModeBase* GameMode = Cast<AAGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (nullptr == GameMode)
		return false;
	AAGHUD* hud = Cast<AAGHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (!IsValid(hud))
		return false;
	UMainWidget* mainWidget = hud->mMainWidget;
	if (!IsValid(hud))
		return false;
	UInventoryWidget* inventoryWidget = mainWidget->GetInventoryWidget();
	inventoryWidget->AddItemByKey(_itemID);


	return true;
}

EITEM_ID AAGPlayer::SelectItem()
{
	int32 randomValue = FMath::RandRange(1, 2);
	return EITEM_ID::POTION_HP_MIN;

	if (randomValue == 1)
	{
		return EITEM_ID::POTION_HP_MIN;
	}
	else if (randomValue == 2)
	{
		return EITEM_ID::POTION_MP_MIN;
	}
	return EITEM_ID::END;
}

void AAGPlayer::SetSkillState(ESkillState NewState)
{
	mSkillState = NewState;
}


void AAGPlayer::SetActionState(EActionState NewActionState, bool IsStateOn)
{
	if (IsStateOn)
	{
		if (NewActionState == EActionState::EAS_Idle)
			mStateType |= (1 << static_cast<uint8>(EActionState::EAS_Idle));
		else if (NewActionState == EActionState::EAS_Move)
			mStateType |= (1 << static_cast<uint8>(EActionState::EAS_Move));
		else if (NewActionState == EActionState::EAS_Jump)
			mStateType |= (1 << static_cast<uint8>(EActionState::EAS_Jump));
		else if (NewActionState == EActionState::EAS_Crouch)
			mStateType |= (1 << static_cast<uint8>(EActionState::EAS_Crouch));
		else if (NewActionState == EActionState::EAS_Guard)
			mStateType |= (1 << static_cast<uint8>(EActionState::EAS_Guard));
		else if (NewActionState == EActionState::EAS_Attack_Skill)
			mStateType |= (1 << static_cast<uint8>(EActionState::EAS_Attack_Skill));
		else if (NewActionState == EActionState::EAS_Hit)
			mStateType |= (1 << static_cast<uint8>(EActionState::EAS_Hit));
	}
	else
	{
		if (NewActionState == EActionState::EAS_Idle)
			mStateType &= ~(1 << static_cast<uint8>(EActionState::EAS_Idle));
		else if (NewActionState == EActionState::EAS_Move)
			mStateType &= ~(1 << static_cast<uint8>(EActionState::EAS_Move));
		else if (NewActionState == EActionState::EAS_Jump)
			mStateType &= ~(1 << static_cast<uint8>(EActionState::EAS_Jump));
		else if (NewActionState == EActionState::EAS_Crouch)
			mStateType &= ~(1 << static_cast<uint8>(EActionState::EAS_Crouch));
		else if (NewActionState == EActionState::EAS_Guard)
			mStateType &= ~(1 << static_cast<uint8>(EActionState::EAS_Guard));
		else if (NewActionState == EActionState::EAS_Attack_Skill)
			mStateType &= ~(1 << static_cast<uint8>(EActionState::EAS_Attack_Skill));
		else if (NewActionState == EActionState::EAS_Hit)
			mStateType &= ~(1 << static_cast<uint8>(EActionState::EAS_Hit));
	}

}

bool AAGPlayer::CheckActionState(EActionState ActionState, bool IsPrintViewport)
{
	bool isState = false;
	FString str = "";

	if (ActionState == EActionState::EAS_Idle)
	{
		isState = (mStateType & (1 << static_cast<uint8>(EActionState::EAS_Idle)));
		str = "Idle ";
	}
	else if (ActionState == EActionState::EAS_Move)
	{
		isState = (mStateType & (1 << static_cast<uint8>(EActionState::EAS_Move)));
		str = "Move ";
	}
	else if (ActionState == EActionState::EAS_Jump)
	{
		isState = (mStateType & (1 << static_cast<uint8>(EActionState::EAS_Jump)));
		str = "EAS_Jump2 ";
	}
	else if (ActionState == EActionState::EAS_Crouch)
	{
		isState = (mStateType & (1 << static_cast<uint8>(EActionState::EAS_Crouch)));
		str = "EAS_Crouch2 ";
	}
	else if (ActionState == EActionState::EAS_Guard)
	{
		isState = (mStateType & (1 << static_cast<uint8>(EActionState::EAS_Guard)));
		str = "EAS_Guard2 ";
	}
	else if (ActionState == EActionState::EAS_Attack_Skill)
	{
		isState = (mStateType & (1 << static_cast<uint8>(EActionState::EAS_Attack_Skill)));
		str = "EAS_Attack_Skill ";
	}
	else if (ActionState == EActionState::EAS_Hit)
	{
		isState = (mStateType & (1 << static_cast<uint8>(EActionState::EAS_Hit)));
		str = "EAS_Hit ";
	}

	if (isState)
		str += "On";
	else
		str += "Off";

	if (IsPrintViewport)
	{
		PrintViewport(1.f, FColor::Black, str);
	}

	return isState;
}