// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"

#include "../AGGameInstance.h"
#include "MonsterSpawnPoint.h"
#include "MonsterAnimInstance.h"
#include "MonsterAIController.h"
#include "../Widget/MonsterHpWidget.h"
#include "../Player/CharacterStatComponent.h"


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
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AMonsterAIController::StaticClass();

	mIsAttackEnd = false;
	
	mPatrolIndex = 1;
	mPatrolWaitTime = 0.f;
	mPatrolProgressTime = 0.f;
	mPatrolIndexAddValue = 1;

	mIsPatrolEnable = false;
	
	mPatrolCurrDistance = 0.f;


	// -1: 사용하는 스킬 없음.
	mUsingSkillIndex = -1;
}

void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
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
		skillInfo.isUse = false;
		skillInfo.duration = 0.0f;

		mSkillInfoArray.Add(skillInfo);
	}
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

float AMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	int32 damage = (int32)Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	damage -= mInfo.defensePoint;

	if (damage < 1)
		damage = 1;
	

	mInfo.hp -= damage;

	PrintViewport(4.f, FColor::Red, FString::Printf(TEXT("maxhp: %d, hp: %d, damage: %d"), mInfo.maxHp, mInfo.hp, damage));

	mInfo.hp < 0.0f ? 0.0f : mInfo.hp;

	UMonsterHpWidget* HPWidget = Cast<UMonsterHpWidget>(mWidgetComopnent->GetWidget());
	if (IsValid(HPWidget))
	{
		HPWidget->SetTargetRatio((float)mInfo.hp / mInfo.maxHp);
	}

	if (mInfo.hp <= 0)
	{
		// 다시 충돌되지 않도록.
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		mAnimInst->SetMonsterMotionType(MONSTER_MOTION::DEATH);


		// 동작되고 있던 로직을 멈춘다.
		AAIController* ai = Cast<AAIController>(GetController());

		if (IsValid(ai))
			ai->BrainComponent->StopLogic(TEXT("Death"));


		mSpawnPoint->RemoveMonster(this);
	}
	else
	{



		//---------------------
		// 자기 자신과 DamageCauser 사이의 각도를 구해 각도에 따라 다른 Hit 애니메이션을 재생한다. 
		//---------------------

		FVector targetPosition = DamageCauser->GetActorLocation();
		FVector position = GetActorLocation();
		FVector direction = targetPosition - position;

		direction.Z = 0.f;
		direction.Normalize();

		float innerProduct = FVector::DotProduct(GetActorForwardVector(), direction);
		float degree = UKismetMathLibrary::DegAcos(innerProduct);

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

	return damage;
}

void AMonster::NormalAttackCheck()
{
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

const FMonsterSkillInfo* AMonster::GetSkillInfo()
{
	if (mUsingSkillIndex == -1)
		return nullptr;

	return &mSkillInfoArray[mUsingSkillIndex];
}

