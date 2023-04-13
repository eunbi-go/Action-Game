// Fill out your copyright notice in the Description page of Project Settings.

//---------------------------
// ���� Ŭ����
//---------------------------

#pragma once

#include "EngineMinimal.h"

#include "DrawDebugHelpers.h"	// ������ ���� ��±��.
#include "EngineGlobals.h"
#include "Engine.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Components/SplineComponent.h"

#include "Kismet/KismetMathLibrary.h"

#include <Components\WidgetComponent.h>




#include "UObject/NoExportTypes.h"
#include "BasicInfo.generated.h"

USTRUCT(BlueprintType)
struct FCharacterInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FName	name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	attackPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	defensePoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	hp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	maxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	mp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	maxMp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32	gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float	movingWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float	movingRunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float	movingDashSpeed;
};



USTRUCT(BlueprintType)
struct FPlayerInfo : public FCharacterInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	attackDistance;
};

USTRUCT(BlueprintType)
struct FPlayerTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		FName	name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int32	attackPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int32	defensePoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int32	hp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int32	maxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int32	mp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int32	maxMp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int32	level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int32	exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int32	gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float	movingWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float	movingRunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float	movingDashSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float	attackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		USkeletalMesh* mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		TSubclassOf<UAnimInstance>	playerAnimClass;
};



USTRUCT(BlueprintType)
struct FMonsterInfo : public FCharacterInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float	attackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float	traceDistance;
};



USTRUCT(BlueprintType)
struct FMonsterTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		FName	name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int32	attackPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int32	defensePoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int32	hp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int32	maxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int32	mp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int32	maxMp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int32	level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int32	exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int32	gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float	patrolSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float	traceSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float	attackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float	traceDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		USkeletalMesh* mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		TSubclassOf<UAnimInstance>	animClass;
};



UENUM(BlueprintType)
enum class SKILL_TYPE : uint8
{
	TELEPORT, SPRINT, CONTINUOUS, SLASH, SKILL_TYPE_END
};


USTRUCT(BlueprintType)
struct FSkillInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int32	slotNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		SKILL_TYPE	skillType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int32	minDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		int32	maxDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		class ASkillActor* skillActor;
};



UENUM(BlueprintType)
enum class INPUT_MODE_TYPE : uint8
{
	GAME_ONLY, GAME_UI, INPUT_MODE_TYPE_END
};


UENUM(BlueprintType)
enum class PATROL_END_DIRECTION : uint8
{
	FORWARD, BACK, END
};


UENUM(BlueprintType)
enum class PATROL_TYPE : uint8
{
	POINT, SPLINE, END
};


//----------------------------
// Boss Skill.
//----------------------------

UENUM(BlueprintType)
enum class ESkill_Type : uint8
{
	PASSIVE, ACTIVE, END
};


UENUM(BlueprintType)
enum class ESkill_System : uint8
{
	ATTACK_SINGLE_ONE, ATTACK_SINGE_DURATION, 
	ATTACK_MULTI_ONE, ATTACK_MULTI_DURATION, 
	BUF, END
};


UENUM(BlueprintType)
enum class ESkill_Point_Type : uint8
{
	DAMAGE, HP_RECOVERY, MP_RECOVERY, ATTACK_UP, DEFENSE_UP, END
};


USTRUCT(Atomic, BlueprintType)
struct FSkillPoint
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	ESkill_Point_Type	type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float	value;
};


UENUM(BlueprintType)
enum class ESkill_Effect_Type : uint8
{
	CAST, PLAY, END
};


USTRUCT(Atomic, BlueprintType)
struct FSkillEffect
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		ESkill_Effect_Type	type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		UNiagaraSystem* niagara;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		UParticleSystem* particle;
};


UENUM(BlueprintType)
enum class ESkill_Anim_Type : uint8
{
	IDLE, PATROL, CHASE, ATTACK, DEATH, 
	SKILL1, SKILL2, SKILL3, SKILL4, SKILL5, END
};


UENUM(BlueprintType)
enum class MONSTER_MOTION : uint8
{
	IDLE, PATROL, CHASE, ATTACK, DEATH, HIT, 
	SKILL1, SKILL2, SKILL3, SKILL4, SKILL5, END
};


USTRUCT(BlueprintType)
struct FSkillData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		ESkill_Type	type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		ESkill_System	system;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		FString	name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		FString	description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		TArray<FSkillPoint>	optionArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		TArray<FSkillEffect>	effectArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float	distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		MONSTER_MOTION	animType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float	coolTime;
};


USTRUCT(BlueprintType)
struct FMonsterSkillInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		ESkill_Type	type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		ESkill_System	system;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		FString	name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		FString	description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		TArray<FSkillPoint>	optionArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		TArray<FSkillEffect>	effectArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float	distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		MONSTER_MOTION	animType;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		bool	isUse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float	duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float	coolTime;

	// ��Ÿ���� üũ�ؾ� �ϴ°�.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		bool	isCheckCoolTime;
};









DECLARE_LOG_CATEGORY_EXTERN(AG, Log, All);

// �α� ����� ���� ��ũ�� ����: �Լ��̸�[�ٹ�ȣ]
//  __FUNCTION__: �Լ� �̸��� �����´�.
//  __LINE__: �ش� ���Ͽ����� �� ���� ������ ���´�.
// FString::FromInt(__LINE__): �� ��ȣ�� ������ ���ͼ� ���ڿ��� ������ش�.
#define	LOG_CALLINFO	(FString(__FUNCTION__) + TEXT("[") + FString::FromInt(__LINE__) + TEXT("]"))

// UE_LOG: �𸮾󿡼� �α� ����� ���� ���.
//  ...: �������ڸ� ����ϱ� ����. (������ ������ ���ϴ´�� �߰� ����)
//  %s: ���ڿ��� �޾ƿͼ� ��ü���ֱ� ���� ����Ѵ�. 
//		���ڿ� ������ �����ؾ� ��.
//		-> FString �տ� * �� �ٿ��� �������� �ϸ� FString�� ������ �ִ� ���ڿ� �����͸� ���´�,
//  __VA_ARGS__ : �������ڸ� ���´�.
#define	LOG(Format, ...)	UE_LOG(MYUE, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define	LOGSTRING(Str)		UE_LOG(MYUE, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *Str)


// �������� ����Ʈ�� �α׸� ����ش�.
void PrintViewport(float Time, const FColor& Color, const FString& Text);



/**
 * 
 */
UCLASS()
class AG_API UBasicInfo : public UObject
{
	GENERATED_BODY()
	
};
