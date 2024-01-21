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

#include "../AbilitySystem/AGAttributeSet.h"
#include "../AbilitySystem/AGAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

AItemActor::AItemActor()
{
	PrimaryActorTick.bCanEverTick = true;

	mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	mBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	mCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	mSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mNiagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	mParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));

	mBox->BodyInstance.SetCollisionProfileName(TEXT("Item"));
	mBox->SetActive(true);
	mBox->SetGenerateOverlapEvents(true);

	mSphere->BodyInstance.SetCollisionProfileName(TEXT("Item"));
	mSphere->SetActive(true);
	mSphere->SetGenerateOverlapEvents(true);

	mCapsule->BodyInstance.SetCollisionProfileName(TEXT("Item"));
	mCapsule->SetActive(true);
	mCapsule->SetGenerateOverlapEvents(true);

	SetRootComponent(mRoot);

	mBox->SetupAttachment(mRoot);
	mSphere->SetupAttachment(mRoot);
	mCapsule->SetupAttachment(mRoot);
	mMesh->SetupAttachment(mRoot);
	mNiagara->SetupAttachment(mRoot);
	mParticle->SetupAttachment(mRoot);
	mItemId = EITEM_ID::END;
}

void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	mBox->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OnOverlap);
	mSphere->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OnOverlap);
	mCapsule->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OnOverlap);
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

void AItemActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (Cast<AAGPlayer>(OtherActor))
	{
		if (mItemId == EITEM_ID::COIN)
		{
			UAGGameInstance* gameInst = Cast<UAGGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

			const FItemDataTable* item = gameInst->FindItemInfoTable("Coin");

			UAbilitySystemComponent* targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
			if (targetASC == nullptr)
				return;

			FGameplayEffectContextHandle ecHandle = targetASC->MakeEffectContext();
			ecHandle.AddSourceObject(this);

			const FGameplayEffectSpecHandle esHandle = targetASC->MakeOutgoingSpec(*item->effect, 1.f, ecHandle);
			const FActiveGameplayEffectHandle agpeHandle = targetASC->ApplyGameplayEffectSpecToSelf(*esHandle.Data.Get());

			Destroy();

		}
		else if (Cast<AAGPlayer>(OtherActor)->AddItem(mItemId))
			Destroy();
	}
}

void AItemActor::SetStaticMesh(const FString& _path)
{
	UStaticMesh* mesh = LoadObject<UStaticMesh>(nullptr, *_path);

	if (IsValid(mesh))
		mMesh->SetStaticMesh(mesh);
}

void AItemActor::SetNiagara(UNiagaraSystem* Niagara)
{
	if (IsValid(Niagara))
		mNiagara->SetAsset(Niagara);
}

void AItemActor::SetNiagara(const FString& Path)
{
	UNiagaraSystem* particle = LoadObject<UNiagaraSystem>(nullptr, *Path);

	if (IsValid(particle))
		mNiagara->SetAsset(particle);
}

void AItemActor::SetParticle(UParticleSystem* _particle)
{
	if (IsValid(_particle))
		mParticle->SetTemplate(_particle);
}

void AItemActor::SetParticle(const FString& _path)
{
	UParticleSystem* particle = LoadObject<UParticleSystem>(nullptr, *_path);

	if (IsValid(particle))
		mParticle->SetTemplate(particle);
}
