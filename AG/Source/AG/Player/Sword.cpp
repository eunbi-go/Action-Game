// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"
#include "../Interface/HitInterface.h"
#include "Valkyrie.h"
#include "../Player/CharacterStatComponent.h"
#include "../AbilitySystem/AGAttributeSet.h"
#include "ValkyriePlayerState.h"
#include "../Particle/ParticleNiagara.h"

ASword::ASword()
{
	PrimaryActorTick.bCanEverTick = true;
	mMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	mMesh->SetupAttachment(GetRootComponent());

	mTrail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Trail"));
	mTrail->SetupAttachment(mMesh);

	mBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	mBoxComp->SetupAttachment(mMesh);
	mBoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	mBoxComp->SetCollisionProfileName(TEXT("PlayerSword"));
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
}

void ASword::Equip(USceneComponent* _parent, FName _socketName, AActor* _newOwner, APawn* _newInstigator)
{
	AttachMeshToSocket(_parent, _socketName);

	SetOwner(_newOwner);
	SetInstigator(_newInstigator);
}

void ASword::SetSkeletalMesh(const FString& _path)
{
	USkeletalMesh* mesh = LoadObject<USkeletalMesh>(nullptr, *_path);

	if (IsValid(mesh))
		mMesh->SetSkeletalMesh(mesh);
}

void ASword::AttachMeshToSocket(USceneComponent* _parent, const FName& _socketName)
{
	FAttachmentTransformRules transformRules(EAttachmentRule::SnapToTarget, true);
	mMesh->AttachToComponent(_parent, transformRules, _socketName);
}

void ASword::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector start = mBoxTraceStart->GetComponentLocation();
	const FVector end = mBoxTraceEnd->GetComponentLocation();

	AValkyrie* owner = Cast<AValkyrie>(mOwner);
	AValkyriePlayerState* playerState = owner->GetPlayerState<AValkyriePlayerState>();
	float damage = Cast<UAGAttributeSet>(playerState->GetAttributeSet())->GetmAttack();
	
	TArray<AActor*> actorsToIgnoreArray;
	actorsToIgnoreArray.Add(this);
	for (AActor* actor : mIgnoreActors)
		actorsToIgnoreArray.AddUnique(actor);

	FHitResult boxHit;
	// 라인을 따라 상자를 설치하고 첫 번째 충돌지점 리턴
	UKismetSystemLibrary::BoxTraceSingle(
		this, start, end, FVector(5.f),
		mBoxTraceStart->GetComponentRotation(),
		// 여기에 무엇을 넣는지는 별로 중요하지 않다
		// 박스 트레이스 싱글은 기본적으로 Visility 채널을 기준으로 추적하기 때문
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		actorsToIgnoreArray,	
		EDrawDebugTrace::None,	
		boxHit,
		true	
	);

	if (boxHit.GetActor())
	{
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
				boxHit.ImpactPoint,
				boxHit.ImpactNormal.Rotation(),
				SpawnParam);

		Particle->SetParticle(TEXT("NiagaraSystem'/Game/Hack_And_Slash_FX/VFX_Niagara/Impacts/NS_Holy_Slash_Impact.NS_Holy_Slash_Impact'"));

	}
}

void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

