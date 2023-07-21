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

	// 현재 스킬을 사용하는 중인가.
	mIsUsingSkill = false;

	mTarget = nullptr;
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
		skillInfo.isUse = false;	// 사용중인가.
		skillInfo.duration = 0.0f;
		skillInfo.coolTime = data->coolTime;
		skillInfo.isCheckCoolTime = false;

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

	if (!mIsUsingSkill)
		UseSkill(DeltaTime);


	if (!target)
		return;

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

	PrintViewport(0.5f, FColor::Blue, FString::Printf(TEXT("distance: %f"), distance));
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
	//PrintViewport(1.f, FColor::Orange, TEXT("GetHit"));
	PlayHitMontage(_impactPoint);
}

float AMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	int32 damage = (int32)Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	damage -= mInfo.defensePoint;

	if (damage < 1)
		damage = 1;
	
	int32 randomValue = FMath::RandRange(10, 20);

	damage = randomValue;

	mInfo.hp -= damage;

	//PrintViewport(4.f, FColor::Red, FString::Printf(TEXT("maxhp: %d, hp: %d, damage: %d"), mInfo.maxHp, mInfo.hp, damage));

	mInfo.hp < 0.0f ? 0.0f : mInfo.hp;

	if (IsValid(mWidgetComopnent))
	{
		UMonsterHpWidget* HPWidget = Cast<UMonsterHpWidget>(mWidgetComopnent->GetWidget());
		if (IsValid(HPWidget))
		{
			HPWidget->SetTargetRatio((float)mInfo.hp / mInfo.maxHp);
		}

		if (Cast<AFengMao>(this))
		{
			AAGGameModeBase* GameMode = Cast<AAGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
			UMainWidget* MainHUD = GameMode->GetMainWidget();
			MainHUD->BossInfoOnOff(false);
		}
	}

	if (mInfo.hp <= 0)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		mAnimInst->SetMonsterMotionType(MONSTER_MOTION::DEATH);


		// 동작되고 있던 로직을 멈춘다.
		AAIController* ai = Cast<AAIController>(GetController());
		if (IsValid(ai))
			ai->BrainComponent->StopLogic(TEXT("Death"));


		mSpawnPoint->RemoveMonster(this);
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

void AMonster::UseSkill(float _deltaTime)
{
	AMonsterAIController* aiController = Cast<AMonsterAIController>(GetController());

	AActor* target = Cast<AActor>(aiController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (!target)
		return;


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
	TArray<int32> enableSkillIndexArray;

	for (int32 i = 0; i < skillCount; ++i)
	{
		// 현재 사용중이면 pass.
		if (mSkillInfoArray[i].isUse || mSkillInfoArray[i].isCheckCoolTime)
			continue;


		if (!mIsUsingSkill && distance <= mSkillInfoArray[i].distance)
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
		aiController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsSkillEnable"), false);
		return;
	}

	int32 randomIndexValue = FMath::RandRange(0, enableSkillCount);
	
	if (randomIndexValue == enableSkillCount)
		randomIndexValue--;

	mSkillInfoArray[enableSkillIndexArray[randomIndexValue]].isUse = true;
	mSkillInfoArray[enableSkillIndexArray[randomIndexValue]].isCheckCoolTime = true;
	mIsUsingSkill = true;
	mUsingSkillIndex = enableSkillIndexArray[randomIndexValue];

	aiController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsSkillEnable"), true);

	enableSkillIndexArray.RemoveAll([](int32) {return true; });
}

void AMonster::CheckSkillCoolTime(float _deltaTime)
{
	int32 skillCount = mSkillInfoArray.Num();

	for (int32 i = 0; i < skillCount; ++i)
	{
		if (mSkillInfoArray[i].isCheckCoolTime)
		{
			mSkillInfoArray[i].duration += _deltaTime;
			
			if (mSkillInfoArray[i].duration >= mSkillInfoArray[i].coolTime)
			{
				PrintViewport(3.f, FColor::Blue, TEXT("cooltime end"));

				mSkillInfoArray[i].duration = 0.0f;
				mSkillInfoArray[i].isUse = false;
				mSkillInfoArray[i].isCheckCoolTime = false;
			}
		}
	}
}

void AMonster::SelectSkill(TArray<int32> _enableSkillIndexArray)
{

}

void AMonster::SetRotationToTarget()
{
	AMonsterAIController* aiController = Cast<AMonsterAIController>(GetController());

	if (IsValid(aiController))
	{
		AActor* target = Cast<AActor>(aiController->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

		PrintViewport(1.f, FColor::Blue, TEXT("isvalid before"));

		//if (IsValid(target))
		//{
			FVector direction = mTarget->GetActorLocation() - GetActorLocation();
			FRotator rot = FRotationMatrix::MakeFromX(direction.GetSafeNormal2D()).Rotator();

			PrintViewport(1.f, FColor::Red, TEXT("SetActor"));

			SetActorRotation(FMath::RInterpTo(GetActorRotation(), rot, GetWorld()->GetDeltaSeconds(), 10.f));
		//}
	}
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
	aiController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsSkillEnable"), true);

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
	aiController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsSkillEnable"), false);

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
	{
		UseSkill(GetWorld()->GetDeltaSeconds());
	}

	if (mUsingSkillIndex == -1)
		return nullptr;

	return &mSkillInfoArray[mUsingSkillIndex];
}

ACharacter* AMonster::GetBlackboardTarget()
{
	return mTarget;
	//AMonsterAIController* aiCotroller = Cast<AMonsterAIController>(GetController());

	//ACharacter* target = Cast<ACharacter>(aiCotroller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	//if (IsValid(target))
	//	return target;

	//return nullptr;
}

