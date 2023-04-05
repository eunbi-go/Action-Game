// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"

#include "../AGGameInstance.h"
#include "MonsterSpawnPoint.h"
#include "MonsterAnimInstance.h"

AMonster::AMonster()
{
	PrimaryActorTick.bCanEverTick = true;

	//-------------------
	// 충돌 세팅
	//-------------------
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);
	GetCapsuleComponent()->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;


	// 데미지 수신 가능.
	SetCanBeDamaged(true);
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

void AMonster::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
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

		mAnimInst->SetMonsterMotion(MONSTER_MOTION::DEATH);

		mSpawnPoint->RemoveMonster(this);
	}

	return damage;
}

