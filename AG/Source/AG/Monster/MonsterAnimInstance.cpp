// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnimInstance.h"

#include "Monster.h"
#include "MonsterAIController.h"
#include "FengMao.h"
#include "../Basic/ItemActor.h"
#include "../Player/PlayerCharacter.h"


UMonsterAnimInstance::UMonsterAnimInstance()
{
	mMonsterMotionType = MONSTER_MOTION::IDLE;

	mHitAdditive = 0.f;
	mHitMontageIndex = 0;
	mHitDirection = TEXT("");

	mIsHit = false;
	mIsSkillEnd = true;
	mCurSkillMontagIndex = -1;
}

void UMonsterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}

void UMonsterAnimInstance::AnimNotify_DeathEnd()
{
	FVector position = TryGetPawnOwner()->GetActorLocation();
	FRotator rotation = TryGetPawnOwner()->GetActorRotation();

	// 아이템 생성.

	for (int32 i = 0; i < 3; ++i)
	{
		int32 randomValue = FMath::RandRange(1, 2);
		float randomX = FMath::RandRange(10.0f, 100.0f);
		float randomY = FMath::RandRange(10.0f, 100.0f);

		// 코인.
		if (randomValue == 1)
		{
			FActorSpawnParameters	params;
			params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			AMonsterAIController* aiCotroller = Cast<AMonsterAIController>(TryGetPawnOwner()->GetController());

			ACharacter* target = Cast<ACharacter>(aiCotroller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

			if (!IsValid(target))
				return;

			position.X += randomX;
			position.Y += randomY;

			AItemActor* item = GetWorld()->SpawnActor<AItemActor>(
				position,
				rotation,
				params);

			item->SetStaticMesh(TEXT("StaticMesh'/Game/CharacterBodyFX/Meshes/SM_Coin.SM_Coin'"));
			item->GetMesh()->SetRelativeScale3D(FVector(10.f));
			item->GetBoxComponent()->SetBoxExtent(FVector(15.0f));
			item->SetItemId(EITEM_ID::COIN);
			item->mOnHitt.AddDynamic(Cast<APlayerCharacter>(target), &APlayerCharacter::AddItem);
		}

		// 랜덤 아이템.
		else if (randomValue == 2)
		{
			FActorSpawnParameters	params;
			params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			AMonsterAIController* aiCotroller = Cast<AMonsterAIController>(TryGetPawnOwner()->GetController());

			ACharacter* target = Cast<ACharacter>(aiCotroller->GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

			if (!IsValid(target))
				return;

			position.X -= randomX;
			position.Y -= randomY;

			AItemActor* item = GetWorld()->SpawnActor<AItemActor>(
				position,
				rotation,
				params);

			item->SetStaticMesh(TEXT("StaticMesh'/Game/CharacterBodyFX/Meshes/SM_Diamond.SM_Diamond'"));
			item->GetMesh()->SetRelativeScale3D(FVector(7.f));
			item->GetBoxComponent()->SetBoxExtent(FVector(35.0f));
			item->SetItemId(EITEM_ID::END);
			item->mOnHitt.AddDynamic(Cast<APlayerCharacter>(target), &APlayerCharacter::AddItem);
		}
	}

	// 몬스터 삭제.
	Cast<AMonster>(TryGetPawnOwner())->DestroyMonster();
}

void UMonsterAnimInstance::AnimNotify_HitEnd()
{
	mHitAdditive = 0.f;
	mIsHit = false;
	AMonster* monster = Cast<AMonster>(TryGetPawnOwner());
	AMonsterAIController* controller = Cast<AMonsterAIController>(monster->GetController());
	controller->GetBlackboardComponent()->SetValueAsBool(TEXT("IsHitEnd"), false);
	monster->SetIsAttackEnd(true);
	PrintViewport(10.f, FColor::Purple, TEXT("hit end"));
}

void UMonsterAnimInstance::AnimNotify_Attack()
{
}

void UMonsterAnimInstance::AnimNotify_AttackEnd()
{
	AMonster* monster = Cast<AMonster>(TryGetPawnOwner());
	//mMonsterMotionType = MONSTER_MOTION::IDLE;
	if (IsValid(monster))
		monster->SetIsAttackEnd(true);
}

void UMonsterAnimInstance::AnimNotify_AttackCheck()
{
	AMonster* monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(monster))
		monster->NormalAttackCheck();
}

void UMonsterAnimInstance::AnimNotify_SkillEnd()
{
	Montage_SetPlayRate(mSkillMontageArray[mCurSkillMontagIndex], 1.f);
	mCurSkillMontagIndex = -1;

	AMonster* monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(monster))
	{
		monster->ClearUsingSkill();
		monster->SetIsAttackEnd(true);
		mIsSkillEnd = true;
	}
}

void UMonsterAnimInstance::AnimNotify_Skill1()
{
	AMonster* monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(monster))
	{
		Cast<AFengMao>(monster)->StartJump();
	}
}

void UMonsterAnimInstance::AnimNotify_Skill1End()
{
	AMonster* monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(monster))
	{
		Cast<AFengMao>(monster)->mSkill1MoveStart.Broadcast(Cast<APlayerCharacter>(monster->GetBlackboardTarget()));

	}
}

void UMonsterAnimInstance::AnimNotify_Skill3()
{
	AMonster* monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(monster))
	{
		monster->Skill3();
	}
}

void UMonsterAnimInstance::AnimNotify_Skill4()
{
	AMonster* monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(monster))
	{
		monster->Skill4();
	}
}

void UMonsterAnimInstance::AnimNotify_PlayRataDown()
{
	PrintViewport(2.f, FColor::Yellow, TEXT("AnimNotify_PlayRataDown"));
	Montage_SetPlayRate(mSkillMontageArray[mCurSkillMontagIndex], 0.2f);
}

void UMonsterAnimInstance::AnimNotify_PlayRataReset()
{
	PrintViewport(2.f, FColor::Yellow, TEXT("AnimNotify_PlayRataReset"));
	Montage_SetPlayRate(mSkillMontageArray[mCurSkillMontagIndex], 1.f);
}

void UMonsterAnimInstance::Hit()
{
	if (!IsValid(mHitMontageArray[mHitMontageIndex]))
		return;

	mHitAdditive = 1.f;

	if (!Montage_IsPlaying(mHitMontageArray[mHitMontageIndex]))
	{
		PrintViewport(10.f, FColor::Purple, TEXT("hitttttttt"));
		Montage_SetPosition(mHitMontageArray[mHitMontageIndex], 0.f);
		Montage_Play(mHitMontageArray[mHitMontageIndex]);
	}
}

void UMonsterAnimInstance::SetHitDirection(FString _value)
{
	mHitDirection = _value;
	mIsHit = true;

	if (_value == TEXT("Front"))
	{
		mHitMontageIndex = 0;
	}
	else if (_value == TEXT("Back"))
	{
		mHitMontageIndex = 1;
	}
	else if (_value == TEXT("Left"))
	{
		mHitMontageIndex = 2;
	}
	else if (_value == TEXT("Right"))
	{
		mHitMontageIndex = 3;
	}

	
	AMonster* monster = Cast<AMonster>(TryGetPawnOwner());
	AMonsterAIController* controller = Cast<AMonsterAIController>(monster->GetController());
	controller->GetBlackboardComponent()->SetValueAsBool(TEXT("IsHitEnd"), true);
	monster->SetIsAttackEnd(true);
	Hit();
	//PrintViewport(2.f, FColor::Blue, mHitDirection);
}

void UMonsterAnimInstance::Attack()
{
	mMonsterMotionType = MONSTER_MOTION::ATTACK;
	AMonster* monster = Cast<AMonster>(TryGetPawnOwner());
	if (IsValid(monster))
	{
		monster->SetIsAttackEnd(false);
	}
	if (!Montage_IsPlaying(mAttackMontage))
	{
		Montage_SetPosition(mAttackMontage, 0.f);
		Montage_Play(mAttackMontage);
	}
}

void UMonsterAnimInstance::SetMonsterMotionType(MONSTER_MOTION _motion)
{
	mMonsterMotionType = _motion; 
	int32 index = 0;
	mIsSkillEnd = true;

	if (mMonsterMotionType == MONSTER_MOTION::SKILL1)
	{
		mCurSkillMontagIndex = 0;
		mIsSkillEnd = false;
	}
	if (mMonsterMotionType == MONSTER_MOTION::SKILL2)
	{
		mCurSkillMontagIndex = 1;
		mIsSkillEnd = false;
	}
	if (mMonsterMotionType == MONSTER_MOTION::SKILL3)
	{
		mCurSkillMontagIndex = 2;
		mIsSkillEnd = false;
	}
	if (mMonsterMotionType == MONSTER_MOTION::SKILL4)
	{
		mCurSkillMontagIndex = 3;
		mIsSkillEnd = false;
	}

	if (!mIsSkillEnd)
	{
		if (!Montage_IsPlaying(mSkillMontageArray[mCurSkillMontagIndex]))
		{
			AMonster* monster = Cast<AMonster>(TryGetPawnOwner());

			monster->SetIsAttackEnd(false);

			//if (IsValid(monster))
			//	monster->SetRotationToTarget();

			Montage_SetPosition(mSkillMontageArray[mCurSkillMontagIndex], 0.f);
			Montage_Play(mSkillMontageArray[mCurSkillMontagIndex]);
		}
	}
}
