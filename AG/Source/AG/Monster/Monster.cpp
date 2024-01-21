// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"

#include "../AGGameInstance.h"
#include "MonsterSpawnPoint.h"
#include "MonsterAnimInstance.h"
#include "MonsterAIController.h"
#include "../AGGameInstance.h"
#include "../Widget/MonsterHpWidget.h"
#include "../Player/CharacterStatComponent.h"
#include "../Player/PlayerCharacter.h"
#include "../AGGameModeBase.h"
#include "../Widget/MainWidget.h"
#include "Math/UnrealMathUtility.h"
#include "../Basic/ItemActor.h"
#include "FengMao.h"
#include "../Player/Valkyrie.h"
#include "../AbilitySystem/AGAbilitySystemComponent.h"
#include "../AbilitySystem/AGAttributeSet.h"
#include "../Widget/HUD/AGHUD.h"

AMonster::AMonster()
{
	PrimaryActorTick.bCanEverTick = true;

	//-------------------
	// 충돌 세팅.
	//-------------------
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);
	GetCapsuleComponent()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;


	// 데미지 수신 가능.
	SetCanBeDamaged(true);



	mWidgetComopnent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetCom"));
	mWidgetComopnent->SetupAttachment(GetMesh());
	mWidgetComopnent->SetRelativeLocation(FVector(0.0f, 0.0f, 200.f));
	mWidgetComopnent->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> ui(TEXT("WidgetBlueprint'/Game/Blueprints/UMG/UI_Monster.UI_Monster_C'"));
	if (ui.Succeeded())
	{
		mWidgetComopnent->SetWidgetClass(ui.Class);
		mWidgetComopnent->SetDrawSize(FVector2D(130.0f, 50.f));
	}

	//-------------------
	// AI Controller 지정.
	//-------------------
	// 레벨에 배치하거나 새롭게 생성되는 Monster마다 
	// AMonsterAIController 객체가 생성되고, 
	// AMonsterAIController 의 지배를 받는다.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AMonsterAIController::StaticClass();


	
	mPatrolIndex = 1;
	mPatrolWaitTime = 0.f;
	mPatrolProgressTime = 0.f;
	mPatrolIndexAddValue = 1;

	mIsPatrolEnable = false;
	
	mPatrolCurrDistance = 0.f;


	// -1: 사용하는 스킬 없음.
	mUsingSkillIndex = -1;

	// 현재 스킬을 사용하는 중인가.
	mIsUsingSkill = false;

	mTarget = nullptr;

	mAttackType = EAttackType::EAT_End;

	mAbilitySystemComp = CreateDefaultSubobject<UAGAbilitySystemComponent>("AbilitySystemComp");
	mAbilitySystemComp->SetIsReplicated(true);
	mAbilitySystemComp->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	mAttributeSet = CreateDefaultSubobject<UAGAttributeSet>("AttributeSet");
}

void AMonster::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
	


	UAGGameInstance* gameInst = GetWorld()->GetGameInstance<UAGGameInstance>();

	//------------------
	// Info.
	//------------------

	const FMonsterTableInfo* info = gameInst->FindMonsterTable(mMonsterTableRowName);

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
		mInfo.traceDistance = info->traceDistance;
		mInfo.attackDistance = info->attackDistance;
		mInfo.movingWalkSpeed = info->patrolSpeed;
		mInfo.movingRunSpeed = info->traceSpeed;

		GetCharacterMovement()->MaxWalkSpeed = mInfo.movingWalkSpeed;

		GetMesh()->SetSkeletalMesh(info->mesh);
		GetMesh()->SetAnimInstanceClass(info->animClass);
	}

	mAnimInst = Cast<UMonsterAnimInstance>(GetMesh()->GetAnimInstance());



	//------------------
	// Widget.
	//------------------

	UMonsterHpWidget* HPWidget = Cast<UMonsterHpWidget>(mWidgetComopnent->GetWidget());
	if (IsValid(HPWidget))
	{
		HPWidget->SetTargetRatio(1.f);
	}



	//------------------
	// Skill.
	//------------------
	
	int32 skillCount = mSkillNameArray.Num();

	for (int32 i = 0; i < skillCount; ++i)
	{
		const FSkillData* data = gameInst->FindMonsterSkillTable(mSkillNameArray[i]);

		FMonsterSkillInfo	skillInfo;
		skillInfo.type = data->type;
		skillInfo.system = data->system;
		skillInfo.name = data->name;
		skillInfo.description = data->description;
		skillInfo.optionArray = data->optionArray;
		skillInfo.effectArray = data->effectArray;
		skillInfo.distance = data->distance;
		skillInfo.animType = data->animType;
		skillInfo.isUse = false;	// 사용중인가.
		skillInfo.duration = 0.0f;
		skillInfo.coolTime = data->coolTime;
		skillInfo.isCheckCoolTime = false;

		mSkillInfoArray.Add(skillInfo);
	}
}

void AMonster::InitAbilityActorInfo()
{
	// 소유자와 아바타 모두 본인.
	mAbilitySystemComp->InitAbilityActorInfo(this, this);
	// 캐릭터는 ASC에 의존함. ASC는 캐릭터에 대해 알 필요X
	Cast<UAGAbilitySystemComponent>(mAbilitySystemComp)->AbilityActorInfoSet();
}

void AMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//-----------------------------
	// 순찰 상태일 경우, 이동양을 구한다.
	//-----------------------------

	if (mIsPatrolEnable)
	{
		mPatrolCurrDistance += (GetCharacterMovement()->MaxWalkSpeed * DeltaTime * mPatrolIndexAddValue);

		if (GetIsPatrolPointArrive())
		{
			if (mPatrolIndexAddValue == 1)
				mPatrolCurrDistance = mPatrolIndex * mPatrolCellDistance;

			else
				mPatrolCurrDistance = mPatrolIndex * mPatrolCellDistance;

			mIsPatrolEnable = false;
		}
	}



	//-----------------------------
	// 타겟 세팅.
	//-----------------------------
	AMonsterAIController* aiCotroller = Cast<AMonsterAIController>(GetController());
	AActor* target = Cast<AActor>(aiCotroller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (IsValid(target))
		mTarget = Cast<ACharacter>(target);



	//-----------------------------
	// 스킬.
	//-----------------------------

	CheckSkillCoolTime(DeltaTime);

	//if (!mIsUsingSkill && mAnimInst->GetIsSkillEnd())
	//	UseSkill(DeltaTime);


	if (!target)
		return;
}

void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// 이 함수는 MonsterAIController 보다 먼저 호출된다.
void AMonster::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//------------------
	// Monster 용 AIController를 설정하고, 그에 맞는 BB, BT 도 함께 지정한다.
	//------------------

	AMonsterAIController* aiController = Cast<AMonsterAIController>(NewController);

	if (IsValid(aiController))
	{
		aiController->SetBehaviorTree(TEXT("BehaviorTree'/Game/Blueprints/Monster/AI/BT_Monster.BT_Monster'"));
		aiController->SetBlackboard(TEXT("BlackboardData'/Game/Blueprints/Monster/AI/BB_Monster.BB_Monster'"));
	}
}

void AMonster::UnPossessed()
{
	Super::UnPossessed();
}

void AMonster::GetHit(const FVector& _impactPoint)
{
	MONSTER_MOTION motion = mAnimInst->GetMonsterMotionType();
	if (motion == MONSTER_MOTION::SKILL1 || motion == MONSTER_MOTION::SKILL2
		|| motion == MONSTER_MOTION::SKILL3 || motion == MONSTER_MOTION::SKILL4
		|| motion == MONSTER_MOTION::SKILL5)
		return;
	if (!mAnimInst->GetIsSkillEnd() || !mIsAttackEnd)
		return;

	PlayHitMontage(_impactPoint);
}


float AMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	int32 damage = (int32)Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	damage -= Cast<UAGAttributeSet>(mAttributeSet)->GetmDefense();
	if (damage < 1)
		damage = 1;
	
	int32 randomValue = FMath::RandRange(10, 20);
	damage -= randomValue;
	damage = fabsf(damage);

	Cast<UAGAttributeSet>(mAttributeSet)->SetmHp(Cast<UAGAttributeSet>(mAttributeSet)->GetmHp() - damage);


	

	if (Cast<UAGAttributeSet>(mAttributeSet)->GetmHp() <= 0)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		mAnimInst->SetMonsterMotionType(MONSTER_MOTION::DEATH);


		// 동작되고 있던 로직을 멈춘다.
		AAIController* ai = Cast<AAIController>(GetController());
		if (IsValid(ai))
			ai->BrainComponent->StopLogic(TEXT("Death"));


		mSpawnPoint->RemoveMonster(this);
	}

	if (IsValid(mWidgetComopnent))
	{
		UMonsterHpWidget* HPWidget = Cast<UMonsterHpWidget>(mWidgetComopnent->GetWidget());
		if (IsValid(HPWidget))
		{
			HPWidget->SetTargetRatio((float)Cast<UAGAttributeSet>(mAttributeSet)->GetmHp() / Cast<UAGAttributeSet>(mAttributeSet)->GetmMaxHp());
		}

		/*if (Cast<AFengMao>(this))
		{
			AAGGameModeBase* GameMode = Cast<AAGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
			AAGHUD* hud = Cast<AAGHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
			if (!IsValid(hud))
				return damage;
			UMainWidget* mainWidget = hud->mMainWidget;
			if (!IsValid(mainWidget))
				return damage;
			mainWidget->BossInfoOnOff(false);
		}*/
	}

	return damage;
}

void AMonster::PlayHitMontage(const FVector& _impactPoint)
{
	FVector position = GetActorLocation();
	FVector impactPosition = FVector(_impactPoint.X, _impactPoint.Y, position.Z);
	FVector direction = (impactPosition - position).GetSafeNormal();


	float innerProduct = FVector::DotProduct(GetActorForwardVector(), direction);
	float degree = UKismetMathLibrary::Acos(innerProduct);
	degree = FMath::RadiansToDegrees(degree);

	FVector outProduct = FVector::CrossProduct(GetActorForwardVector(), direction);
	float sign = UKismetMathLibrary::SignOfFloat(outProduct.Z);

	float angle = sign * degree;



	FString angleString = TEXT("");

	// 오른쪽.
	if (angle >= 0.f)
	{
		if (degree >= 50.f && angle <= 130.f)
			angleString = TEXT("Right");
		else if (degree < 50.f)
			angleString = TEXT("Front");
		else
			angleString = TEXT("Back");
	}

	// 왼쪽
	else if (angle < 0.f)
	{
		if (degree <= -50.f && angle >= -130.f)
			angleString = TEXT("Left");
		else if (degree > -50.f)
			angleString = TEXT("Front");
		else
			angleString = TEXT("Back");
	}

	mAnimInst->SetHitDirection(angleString);
}



void AMonster::Skill1()
{
}

void AMonster::Skill2()
{
}

void AMonster::Skill3()
{
}

void AMonster::Skill4()
{
}

bool AMonster::CheckEnableSkill()
{
	AMonsterAIController* aiController = Cast<AMonsterAIController>(GetController());
	AActor* target = Cast<AActor>(aiController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (!target)
		return false;


	float	capsuleHalfHeight = 0.f;

	if (Cast<ACharacter>(target))
		capsuleHalfHeight = Cast<ACharacter>(target)->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	FVector targetPosition = target->GetActorLocation();
	targetPosition.Z -= capsuleHalfHeight;

	FVector position = GetActorLocation();
	position.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	float distance = (float)FVector::Distance(targetPosition, position);
	distance -= GetCapsuleComponent()->GetScaledCapsuleRadius();
	distance -= Cast<ACharacter>(target)->GetCapsuleComponent()->GetScaledCapsuleRadius();


	enableSkillIndexArray.RemoveAll([](int32) {return true; });

	int32 skillCount = mSkillInfoArray.Num();
	

	for (int32 i = 0; i < skillCount; ++i)
	{
		// 현재 사용중이면 pass.
		if (mSkillInfoArray[i].isUse || mSkillInfoArray[i].isCheckCoolTime)
			continue;


		if (!mIsUsingSkill && mAnimInst->GetIsSkillEnd() && distance <= mSkillInfoArray[i].distance)
		{
			enableSkillIndexArray.Add(i);
		}
	}

	//---------------------
	// 스킬 사용.
	//---------------------
	int32 enableSkillCount = enableSkillIndexArray.Num();

	if (enableSkillCount == 0)
	{
		return false;
	}
	return true;
}

const FMonsterSkillInfo* AMonster::UseSkill()
{
	int32 enableSkillCount = enableSkillIndexArray.Num();
	int32 randomIndexValue = FMath::RandRange(0, enableSkillCount);
	
	if (randomIndexValue == enableSkillCount)
		randomIndexValue--;

	mSkillInfoArray[enableSkillIndexArray[randomIndexValue]].isUse = true;
	mSkillInfoArray[enableSkillIndexArray[randomIndexValue]].isCheckCoolTime = true;
	mIsUsingSkill = true;
	mUsingSkillIndex = enableSkillIndexArray[randomIndexValue];

	enableSkillIndexArray.RemoveAll([](int32) {return true; });

	return &mSkillInfoArray[mUsingSkillIndex];
}

// 쿨타임만 확인한다.
void AMonster::CheckSkillCoolTime(float _deltaTime)
{
	int32 skillCount = mSkillInfoArray.Num();
	AMonsterAIController* aiController = Cast<AMonsterAIController>(GetController());
	AActor* target = Cast<AActor>(aiController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!target)
		return;

	for (int32 i = 0; i < skillCount; ++i)
	{
		if (mSkillInfoArray[i].isCheckCoolTime)
		{
			mSkillInfoArray[i].duration += _deltaTime;
			
			if (mSkillInfoArray[i].duration >= mSkillInfoArray[i].coolTime)
			{
				PrintViewport(0.5f, FColor::Red, FString::Printf(TEXT("%d cooltime end"), i));

				mSkillInfoArray[i].duration = 0.0f;
				mSkillInfoArray[i].isUse = false;
				mSkillInfoArray[i].isCheckCoolTime = false;
			}
		}

	}
}

bool AMonster::IsEnableUseSkill()
{
	AMonsterAIController* aiController = Cast<AMonsterAIController>(GetController());

	AActor* target = Cast<AActor>(aiController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!target)
		return false;


	float	capsuleHalfHeight = 0.f;

	if (Cast<ACharacter>(target))
		capsuleHalfHeight = Cast<ACharacter>(target)->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	FVector targetPosition = target->GetActorLocation();
	targetPosition.Z -= capsuleHalfHeight;

	FVector position = GetActorLocation();
	position.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	float distance = (float)FVector::Distance(targetPosition, position);
	distance -= GetCapsuleComponent()->GetScaledCapsuleRadius();
	distance -= Cast<ACharacter>(target)->GetCapsuleComponent()->GetScaledCapsuleRadius();



	int32 skillCount = mSkillInfoArray.Num();

	for (int32 i = 0; i < skillCount; ++i)
	{
		// 현재 사용중이면 pass.
		if (mSkillInfoArray[i].isUse || mSkillInfoArray[i].isCheckCoolTime)
			continue;
		// 스킬 쿨타임 체크하는중이면 pass 
		if (mSkillInfoArray[i].isCheckCoolTime)
			continue;

		if (!mIsUsingSkill && mAnimInst->GetIsSkillEnd() && distance <= mSkillInfoArray[i].distance)
		{
			return true;
		}
	}

	return false;
}

const FMonsterSkillInfo* AMonster::GetSkillInfo()
{
	if (CheckEnableSkill())
	{
		//UseSkill(GetWorld()->GetDeltaSeconds());
	}

	if (mUsingSkillIndex == -1)
		return nullptr;

	return &mSkillInfoArray[mUsingSkillIndex];
}


void AMonster::NormalAttackCheck()
{
}

void AMonster::ClearUsingSkill()
{
	if (mUsingSkillIndex == -1)
		return;


	mSkillInfoArray[mUsingSkillIndex].isUse = false;
	
	mUsingSkillIndex = -1;
	mIsUsingSkill = false;
	mIsAttackEnd = true;

	AMonsterAIController* aiController = Cast<AMonsterAIController>(GetController());
	//aiController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsSkillEnable"), true);

	//UseSkill(GetWorld()->GetDeltaSeconds()); 
}

void AMonster::ClearAllSkill()
{
	mUsingSkillIndex = -1;
	mIsUsingSkill = false;
	mIsAttackEnd = true;

	int32 skillCount = mSkillInfoArray.Num();

	for (int32 i = 0; i < skillCount; ++i)
	{
		mSkillInfoArray[i].isUse = false;
	}


	AMonsterAIController* aiController = Cast<AMonsterAIController>(GetController());
	//aiController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsSkillEnable"), false);

	//UseSkill(GetWorld()->GetDeltaSeconds());
}

void AMonster::GoNextPatrolPoint()
{
	//-----------------------------
	// 다음 순찰 인덱스를 결정한다.
	//-----------------------------

	mPatrolIndex += mPatrolIndexAddValue;

	if (mPatrolType == PATROL_TYPE::POINT)
	{
		// 마지막 순찰 지점까지 다 돌았다.
		if (mPatrolIndex == mPatrolPointPositionArray.Num())
		{
			switch (mPatrolDirection)
			{
			case PATROL_END_DIRECTION::FORWARD:
				// 다시 처음부터 시작.
				mPatrolIndex = 0;
				break;

			case PATROL_END_DIRECTION::BACK:
				// 마지막부터 역순으로 시작.
				mPatrolIndexAddValue = -1;
				mPatrolIndex = mPatrolPointPositionArray.Num() - 2;
				break;
			}
		}

		else if (mPatrolIndex < 0)
		{
			mPatrolIndexAddValue = 1;
			mPatrolIndex = 1;
		}
	}


	else if (mPatrolType == PATROL_TYPE::SPLINE)
	{
		// 마지막 순찰 지점까지 다 돌았다.
		if (mPatrolIndex == mPatrolSplineCount + 1)
		{
			switch (mPatrolDirection)
			{
			case PATROL_END_DIRECTION::FORWARD:
				// 다시 처음부터 시작한다.
				mPatrolIndex = 1;
				mPatrolCurrDistance -= mPatrolSplineLength;
				break;

			case PATROL_END_DIRECTION::BACK:
				// 현재 위치에서 역순으로 돌아간다.
				mPatrolCurrDistance = mPatrolSplineLength - 100.f - GetCapsuleComponent()->GetScaledCapsuleRadius();
				mPatrolIndexAddValue = -1;
				mPatrolIndex = mPatrolSplineCount - 1;
				break;
			}
		}

		else if (mPatrolIndex < 0)
		{
			mPatrolCurrDistance = 100.f + GetCapsuleComponent()->GetScaledCapsuleRadius();
			mPatrolIndex = 1;
			mPatrolIndexAddValue = 1;
		}
	}
	
}

void AMonster::DestroyMonster()
{
	mSpawnPoint->RemoveMonster(this);
	Destroy();
}

void AMonster::PlaySkillMontage(MONSTER_MOTION motion)
{
}



FVector AMonster::GetPatrolPosition() const
{
	//-----------------------------
	// 순찰해야 하는 위치를 반환한다.
	//-----------------------------

	switch (mPatrolType)
	{
	case PATROL_TYPE::POINT:
		return mPatrolPointPositionArray[mPatrolIndex];

	case PATROL_TYPE::SPLINE:
		return mSpawnPoint->GetSplinePosition(mPatrolCurrDistance);
	}

	return FVector::ZeroVector;
}

FVector AMonster::GetPatrolPointPosition() const
{
	//-----------------------------
	// 순찰해야 하는 위치를 반환한다.
	//-----------------------------

	switch (mPatrolType)
	{
	case PATROL_TYPE::POINT:
		return mPatrolPointPositionArray[mPatrolIndex];

	case PATROL_TYPE::SPLINE:
		return mSpawnPoint->GetSplinePosition(mPatrolIndex * mPatrolCellDistance);
	}

	return FVector::ZeroVector;
}

bool AMonster::GetIsPatrolPointArrive()
{
	//-----------------------------
	// 현재 인덱스까지의 스플라인 길이와 현재까지 이동한 거리를 비교해서 
	// CP 까지 도착했는지 판단한다.
	//-----------------------------

	float distance = 10.f + GetCapsuleComponent()->GetScaledCapsuleRadius();

	if (mPatrolIndexAddValue == 1)
		return mPatrolIndex * mPatrolCellDistance - distance <= mPatrolCurrDistance;

	return mPatrolIndex * mPatrolCellDistance + distance >= mPatrolCurrDistance;
}




