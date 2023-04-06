// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"

#include "../AGGameInstance.h"
#include "MonsterSpawnPoint.h"
#include "MonsterAnimInstance.h"
#include "MonsterAIController.h"


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


	//-------------------
	// AI Controller 지정.
	//-------------------
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AMonsterAIController::StaticClass();

	mIsAttackEnd = false;
	
	mPatrolIndex = 1;
	mPatrolWaitTime = 0.f;
	mPatrolProgressTime = 0.f;
}

void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
	UAGGameInstance* gameInst = GetWorld()->GetGameInstance<UAGGameInstance>();

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
}

void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

	PrintViewport(4.f, FColor::Red, FString::Printf(TEXT("hp: %d, damage: %d"), mInfo.hp, damage));


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
		mAnimInst->Hit();

	return damage;
}

void AMonster::GoNextPatrolPoint()
{
	mPatrolIndex++;

	// 마지막 순찰 지점까지 다 돌았다.
	if (mPatrolIndex == mPatrolPointPositionArray.Num())
	{
		switch (mPatrolDirection)
		{
		case PATROL_END_DIRECTION::FORWARD:
			mPatrolIndex = 0;
			PrintViewport(3.f, FColor::Blue, TEXT("arrive"));
			break;

		case PATROL_END_DIRECTION::BACK:
			mPatrolIndex = -1;
			mPatrolIndex = mPatrolPointPositionArray.Num() - 2;
			break;
		}
	}

	else if (mPatrolIndex < 0)
		mPatrolIndex = 1;
}

