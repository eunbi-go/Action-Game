// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"
#include "../Player/PlayerCharacter.h"
#include "../Manager/InventoryManager.h"
#include "../Widget/InventoryItem.h"
#include "../Widget/InventoryWidget.h"
#include "../Widget/MainWidget.h"
#include "../Widget/ItemData.h"
#include "../AGGameInstance.h"
#include "../AGGameModeBase.h"
#include "../Player/AGPlayer.h"

AItemActor::AItemActor()
{
	PrimaryActorTick.bCanEverTick = true;

	mBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	mBox->BodyInstance.SetCollisionProfileName(TEXT("Item"));
	mBox->SetActive(true);
	mBox->SetGenerateOverlapEvents(true);

	SetRootComponent(mBox);

	mMesh->SetupAttachment(mBox);
	mItemId = EITEM_ID::END;
}

void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	SetOverlapEnable();
}

void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	runningTime += DeltaTime;
	AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
}

float AItemActor::TransformedSin()
{
	return amplitude * FMath::Sin(runningTime * timeConstant);
}

void AItemActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AAGGameModeBase* GameMode = Cast<AAGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (nullptr == GameMode)
		return;

	if (Cast<APlayerCharacter>(OtherActor))
	{
		Cast<APlayerCharacter>(OtherActor)->SetItemId(mItemId);
	}
}

void AItemActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (Cast<APlayerCharacter>(OtherActor))
	//{
	//	Cast<APlayerCharacter>(OtherActor)->SetItemId(mItemId);
	//	mOnHitt.Broadcast(this, SweepResult, OtherActor);
	//}
		if (Cast<AAGPlayer>(OtherActor))
	{
		if (Cast<AAGPlayer>(OtherActor)->AddItem(mItemId))
			Destroy();
	}
}

void AItemActor::SetStaticMesh(const FString& _path)
{
	UStaticMesh* mesh = LoadObject<UStaticMesh>(nullptr, *_path);

	if (IsValid(mesh))
		mMesh->SetStaticMesh(mesh);
}

