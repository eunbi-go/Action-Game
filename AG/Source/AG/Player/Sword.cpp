// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"
#include "../Interface/HitInterface.h"
#include "Valkyrie.h"
#include "../Player/CharacterStatComponent.h"
#include "../AbilitySystem/AGAttributeSet.h"
#include "ValkyriePlayerState.h"
#include "../Particle/ParticleNiagara.h"
#include "DrawDebugHelpers.h"

ASword::ASword()
{
	PrimaryActorTick.bCanEverTick = true;
	mMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	mMesh->SetupAttachment(GetRootComponent());

	mTrail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Trail"));
	mTrail->SetupAttachment(mMesh);

	mBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	mBoxComp->SetupAttachment(mMesh);
	mBoxComp->SetCollisionProfileName(TEXT("PlayerSword"));
	mBoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mBoxComp->SetBoxExtent(FVector(3.f, 5.f, 45.f));
	mBoxComp->SetRelativeLocation(FVector(0.f, 0.f, 70.f));

	mBoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceStart"));
	mBoxTraceStart->SetupAttachment(mMesh);
	mBoxTraceStart->SetRelativeLocation(FVector(0.f, 0.f, 30.f));

	mBoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceEnd"));
	mBoxTraceEnd->SetupAttachment(mMesh);
	mBoxTraceEnd->SetRelativeLocation(FVector(0.f, 0.f, 110.f));
}

void ASword::BeginPlay()
{
	Super::BeginPlay();

	UNiagaraSystem* Particle = LoadObject<UNiagaraSystem>(
		nullptr, 
		TEXT("NiagaraSystem'/Game/sA_SwordTrailSet/Fx/NS_Holy_Trail_1.NS_Holy_Trail_1'")
		/*TEXT("NiagaraSystem'/Game/sA_SwordTrailSet/Fx/NS_Flameslash_3.NS_Flameslash_3'")*/
		/*TEXT("NiagaraSystem'/Game/sA_SwordTrailSet/Fx/NS_Star_2.NS_Star_2'")*/
	);

	if (IsValid(Particle))
		mTrail->SetAsset(Particle);
	mTrail->SetVisibility(false);
	mTrail->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));

	mBoxComp->OnComponentBeginOverlap.AddDynamic(this, &ASword::OnBoxOverlap);
	mBoxComp->bHiddenInGame = false;
}

void ASword::Equip(USceneComponent* _parent, FName _socketName, AActor* _newOwner, APawn* _newInstigator)
{
	AttachMeshToSocket(_parent, _socketName);

	SetOwner(_newOwner);
	SetInstigator(_newInstigator);
}

void ASword::AttachMeshToSocket(USceneComponent* _parent, const FName& _socketName)
{
	FAttachmentTransformRules transformRules(EAttachmentRule::SnapToTarget, true);
	mMesh->AttachToComponent(_parent, transformRules, _socketName);
}

void ASword::SetSkeletalMesh(const FString& _path)
{
	USkeletalMesh* mesh = LoadObject<USkeletalMesh>(nullptr, *_path);

	if (IsValid(mesh))
		mMesh->SetSkeletalMesh(mesh);
}

void ASword::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AValkyrie* owner = Cast<AValkyrie>(mOwner);

	//---------------------------------
	// 타격하면 안되는 액터들을 저장한다.
	//---------------------------------
	TArray<AActor*> actorsToIgnoreArray;
	actorsToIgnoreArray.Add(this);
	actorsToIgnoreArray.Add(owner);
	for (AActor* actor : mIgnoreActors)
		actorsToIgnoreArray.AddUnique(actor);


	FHitResult boxHit;
	const FVector start = mBoxTraceStart->GetComponentLocation();
	const FVector end = mBoxTraceEnd->GetComponentLocation();

	//---------------------------------
	// 검의 시작과 끝 위치를 기준으로 Sword의 물리적 충돌 여부를 확인한다.
	//---------------------------------
	UKismetSystemLibrary::BoxTraceSingle(
		this, 
		start, // mBoxTraceStart->GetComponentLocation();
		end,   // mBoxTraceEnd->GetComponentLocation();
		FVector(3.f),
		mBoxTraceStart->GetComponentRotation(),
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel4),
		false,
		actorsToIgnoreArray,	
		EDrawDebugTrace::None,	
		boxHit,  // FHitResult boxHit;
		true,
		FLinearColor::Blue,
		FLinearColor::Red,
		1.f
	);



	
	if (boxHit.GetActor())
	{
		//---------------------------------
		// 데미지 부여, 피격 애니메이션 재생, 이펙트 스폰
		//---------------------------------
		AValkyriePlayerState* playerState = owner->GetPlayerState<AValkyriePlayerState>();
		float damage = Cast<UAGAttributeSet>(playerState->GetAttributeSet())->GetmStrength();

		UGameplayStatics::ApplyDamage(
			boxHit.GetActor(),
			damage,
			GetInstigator()->GetController(),
			this,
			UDamageType::StaticClass()
		);

		IHitInterface* hitInterface = Cast<IHitInterface>(boxHit.GetActor());
		if (hitInterface)
		{
			hitInterface->GetHit(boxHit.ImpactPoint);
		}
		
		
		mIgnoreActors.AddUnique(boxHit.GetActor());


		FActorSpawnParameters	SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AParticleNiagara* Particle =
			GetWorld()->SpawnActor<AParticleNiagara>(
				boxHit.GetActor()->GetActorLocation(),
				FRotator::ZeroRotator,
				SpawnParam);

		Particle->SetParticle(TEXT("NiagaraSystem'/Game/sA_StylizedAttacksPack/FX/NiagaraSystems/NS_BasicHit.NS_BasicHit'"));
		Particle->SetActorScale3D(FVector(4.f));
	}
}

void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

