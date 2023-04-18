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

AItemActor::AItemActor()
{
	PrimaryActorTick.bCanEverTick = true;

	mBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	mBox->BodyInstance.SetCollisionProfileName(TEXT("Item"));
	mBox->SetNotifyRigidBodyCollision(true);
	mBox->SetSimulatePhysics(true);
	
	SetRootComponent(mBox);

	mMesh->SetupAttachment(mBox);
}

void AItemActor::BeginPlay()
{
	Super::BeginPlay();

	mBox->OnComponentHit.AddDynamic(this, &AItemActor::OnHit);

	//mBox->SetBoxExtent(FVector(15.0f));
}

void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItemActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AAGGameModeBase* GameMode = Cast<AAGGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	// 알아낸 게임모드가 AAGGameModeBase 가 아니라면 캐스팅 실패 == 현재 월드가 메인 레벨이 아니라는 뜻
	if (nullptr == GameMode)
		return;

	if (Cast<APlayerCharacter>(OtherActor))
		mOnHitt.Broadcast(this, Hit, OtherActor);
}

void AItemActor::SetStaticMesh(const FString& _path)
{
	UStaticMesh* mesh = LoadObject<UStaticMesh>(nullptr, *_path);

	if (IsValid(mesh))
		mMesh->SetStaticMesh(mesh);
}

