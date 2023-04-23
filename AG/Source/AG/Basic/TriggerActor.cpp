// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerActor.h"

ATriggerActor::ATriggerActor()
{
	PrimaryActorTick.bCanEverTick = true;

	mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	mBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));

	SetRootComponent(mRoot);
	mBox->SetupAttachment(mRoot);

	mRoot->bVisualizeComponent = true;

	mTriggerId = ETRIGGER_ID::BOX;
}

void ATriggerActor::BeginPlay()
{
	Super::BeginPlay();
	mBox->SetCollisionProfileName(TEXT("PlayerTrigger"));
	mBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	mBox->OnComponentBeginOverlap.AddDynamic(this, &ATriggerActor::BeginOverlap);
	mBox->OnComponentEndOverlap.AddDynamic(this, &ATriggerActor::EndOverlap);
}

void ATriggerActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	//if (mBox)
	//{
	//	mBox->DetachFromParent();
	//	mBox->DestroyComponent();
	//	mBox = nullptr;
	//}

	//mBox = NewObject<UBoxComponent>(this, UBoxComponent::StaticClass(), TEXT("Box"));
	//mBox->RegisterComponent();
	//mBox->AttachToComponent(mRoot, FAttachmentTransformRules::KeepRelativeTransform);


	//ChangeCollisionProfile(TEXT("PlayerTrigger"));
}

void ATriggerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATriggerActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PrintViewport(10.f, FColor::Red, TEXT("ATriggerActor::BeginOverlap"));
	TriggerBegin(SweepResult);
}

void ATriggerActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	PrintViewport(10.f, FColor::Red, TEXT("ATriggerActor::EndOverlap"));
	TriggerEnd();
}

void ATriggerActor::ChangeCollisionProfile(const FString& _profileName)
{
	mBox->SetCollisionProfileName(*_profileName);
	mBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ATriggerActor::TriggerBegin(const FHitResult& _result)
{
}

void ATriggerActor::TriggerEnd()
{
}

