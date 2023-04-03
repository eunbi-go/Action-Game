// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../BasicInfo.h"
#include "GameFramework/Actor.h"
#include "SkillActor.generated.h"

UCLASS()
class AG_API ASkillActor : public AActor
{
	GENERATED_BODY()
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillEnd, ASkillActor*, SkillActor, const FHitResult&, Hit);

public:	
	ASkillActor();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnSkillEnd	mOnSkillEnd;



public:
	void SetSkeletalMesh(const FString& _path);
	void SetStaticMesh(const FString& _path);
	void SetParticle(const FString& _path);
	void SetSound(const FString& _path);
	void SetNiagara(UNiagaraSystem* _niagara);
	void SetNiagara(const FString& _path);
	void SetBoxExtent(const FVector& _extent);
	void SetCollisionProfile(const FName& _profile);


public:
	UNiagaraComponent* GetNiagara() { return mNiagara; }


public:
	template <typename T>
	void AddSkillEndDelegate(T* Obj, void (T::* Func)(ASkillActor*))
	{
		mOnSkillEnd.AddDynamic(Obj, Func);
	}



public:
	//UPROPERTY(BlueprintAssignable)
	//FOnSkillEnd	mOnSkillEnd;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UBoxComponent* mRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* mSkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* mStaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UParticleSystemComponent* mParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UNiagaraComponent* mNiagara;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USoundBase* mSound;
};
