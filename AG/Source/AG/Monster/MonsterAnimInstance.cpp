// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnimInstance.h"

#include "Monster.h"
#include "MonsterAIController.h"
#include "FengMao.h"
#include "../Basic/ItemActor.h"
#include "../Player/PlayerCharacter.h"
#include "../Manager/InventoryManager.h"

UMonsterAnimInstance::UMonsterAnimInstance()
{
	mMonsterMotionType = MONSTER_MOTION::IDLE;

	mHitAdditive = 0.f;
	mHitMontageIndex = 0;
	mHitDirection = TEXT("");

	mIsHit = false;
	mIsSkillEnd = true;
	mCurSkillMontagIndex = -1;

	mItemCount = 0;
}

void UMonsterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (mCurSkillMontagIndex != -1)
	{
		int32 cnt = mSkillMontageArray.Num();
		for (int32 i = 0; i < cnt; ++i)
		{
			//if (Montage_IsPlaying(mSkillMontageArray[i]))
				//mMonsterMotionType = MONSTER_MOTION::SKILL1;
		}
	}
}

void UMonsterAnimInstance::AnimNotify_DeathEnd()
{
	for (int32 i = 0; i < mItemCount; ++i)
		mItems[i]->SetOverlapEnable();
	Cast<AMonster>(TryGetPawnOwner())->SetIsDead(true);
	Cast<AMonster>(TryGetPawnOwner())->DestroyMonster();
}

void UMonsterAnimInstance::AnimNotify_SpawnItem()
{
	FVector position = TryGetPawnOwner()->GetActorLocation();
	FRotator rotation = TryGetPawnOwner()->GetActorRotation();

	mItemCount = FMath::RandRange(1, 5);

	for (int32 i = 0; i < mItemCount; ++i)
	{
		int32 randomItemValue = FMath::RandRange(1, 2);
		float randomXPlus = FMath::RandRange(10.0f, 100.0f);
		float randomYPlus = FMath::RandRange(10.0f, 100.0f);

		float randomXMinus = FMath::RandRange(10.0f, 100.0f);
		float randomYMinus = FMath::RandRange(10.0f, 100.0f);

		FActorSpawnParameters	params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		position.X += randomXPlus;
		position.Y += randomYPlus;

		position.X -= randomXMinus;
		position.Y -= randomYMinus;

		AItemActor* item = GetWorld()->SpawnActor<AItemActor>(
			position,
			rotation,
			params);

		// 코인.
		if (item && randomItemValue == 1)
		{
			item->SetStaticMesh(TEXT("StaticMesh'/Game/CharacterBodyFX/Meshes/SM_Coin.SM_Coin'"));
			item->GetMesh()->SetRelativeScale3D(FVector(10.f));
			item->GetBoxComponent()->SetBoxExtent(FVector(15.0f));
			item->GetBoxComponent()->SetActive(false);
			item->SetItemId(EITEM_ID::COIN);
		}

		// 랜덤 아이템.
		else if (item && randomItemValue == 2)
		{
			item->SetStaticMesh(TEXT("StaticMesh'/Game/CharacterBodyFX/Meshes/SM_Diamond.SM_Diamond'"));
			item->GetMesh()->SetRelativeScale3D(FVector(3.f));
			item->GetBoxComponent()->SetBoxExtent(FVector(35.0f));
			item->SetItemId(EITEM_ID::END);
		}

		mItems.Add(item);
	}
}

void UMonsterAnimInstance::AnimNotify_HitEnd()
{
	mHitAdditive = 0.f;
	mIsHit = false;
	AMonster* monster = Cast<AMonster>(TryGetPawnOwner());
	AMonsterAIController* controller = Cast<AMonsterAIController>(monster->GetController());
	controller->GetBlackboardComponent()->SetValueAsBool(TEXT("IsHitEnd"), false);
	monster->SetIsAttackEnd(true);
	//PrintViewport(10.f, FColor::Purple, TEXT("hit end"));
}

void UMonsterAnimInstance::AnimNotify_Attack()
{
}

void UMonsterAnimInstance::AnimNotify_AttackEnd()
{
	AMonster* monster = Cast<AMonster>(TryGetPawnOwner());
	//mMonsterMotionType = MONSTER_MOTION::IDLE;
	if (IsValid(monster))
	{
		PrintViewport(2.f, FColor::Red, TEXT("AttackEnd"));
		monster->SetIsAttackEnd(true);
		mIsSkillEnd = true;
		monster->mOnAttackEnd.Broadcast();
	}
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
		PrintViewport(5.f, FColor::Orange, FString("AnimNotify_SkillEnd"));
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
		Cast<AFengMao>(monster)->mSkill1MoveStart.Broadcast();
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
	//PrintViewport(2.f, FColor::Yellow, TEXT("AnimNotify_PlayRataDown"));
	Montage_SetPlayRate(mSkillMontageArray[mCurSkillMontagIndex], 0.2f);
}

void UMonsterAnimInstance::AnimNotify_PlayRataReset()
{
	//PrintViewport(2.f, FColor::Yellow, TEXT("AnimNotify_PlayRataReset"));
	Montage_SetPlayRate(mSkillMontageArray[mCurSkillMontagIndex], 1.f);
}

void UMonsterAnimInstance::AnimNotify_SkillStart()
{
	mIsSkillEnd = false;
}

void UMonsterAnimInstance::Hit()
{
	if (!IsValid(mHitMontageArray[mHitMontageIndex]))
		return;

	mHitAdditive = 1.f;

	if (!Montage_IsPlaying(mHitMontageArray[mHitMontageIndex]))
	{
		//PrintViewport(10.f, FColor::Purple, TEXT("hitttttttt"));
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
	if (monster)
	{
		monster->SetIsAttackEnd(true);
	}
	Hit();
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
	bool isSkill = false;

	if (_motion == MONSTER_MOTION::SKILL1)
	{
		AMonster* monster = Cast<AMonster>(TryGetPawnOwner());
		if (IsValid(monster))
		{
			const FMonsterSkillInfo* info = monster->UseSkill();
			mMonsterMotionType = info->animType;
		}
	}

	// 사용할 스킬 선택
	if (mMonsterMotionType == MONSTER_MOTION::SKILL1)
	{
		mCurSkillMontagIndex = 0;
		isSkill = true;
	}
	if (mMonsterMotionType == MONSTER_MOTION::SKILL2)
	{
		mCurSkillMontagIndex = 1;
		isSkill = true;
	}
	if (mMonsterMotionType == MONSTER_MOTION::SKILL3)
	{
		mCurSkillMontagIndex = 2;
		isSkill = true;
	}
	if (mMonsterMotionType == MONSTER_MOTION::SKILL4)
	{
		mCurSkillMontagIndex = 3;
		isSkill = true;
	}

	if (mIsSkillEnd && isSkill && Cast<AMonster>(TryGetPawnOwner())->GetIsAttackEnd())
	{
		if (!Montage_IsPlaying(mSkillMontageArray[mCurSkillMontagIndex]))
		{
			AMonster* monster = Cast<AMonster>(TryGetPawnOwner());

			
			if (mMonsterMotionType == MONSTER_MOTION::SKILL1
				|| mMonsterMotionType == MONSTER_MOTION::SKILL2
				|| mMonsterMotionType == MONSTER_MOTION::SKILL3
				|| mMonsterMotionType == MONSTER_MOTION::SKILL4
				|| mMonsterMotionType == MONSTER_MOTION::SKILL5)
			{
				//mIsSkillEnd = false;
				monster->PlaySkillMontage(mMonsterMotionType);
				monster->SetIsAttackEnd(false);
			}
		}
	}
}
