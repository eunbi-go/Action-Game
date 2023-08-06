// Fill out your copyright notice in the Description page of Project Settings.


#include "Sword.h"
#include "../Interface/HitInterface.h"

// Sets default values
ASword::ASword()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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

void ASword::SetCollisionOnOff(bool _value)
{
	if (_value)
	{
		mBoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		mBoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ASword::BeginPlay()
{
	Super::BeginPlay();

	UNiagaraSystem* Particle = LoadObject<UNiagaraSystem>(
		nullptr, 
		/*TEXT("NiagaraSystem'/Game/sA_SwordTrailSet/Fx/NS_Flameslash_3.NS_Flameslash_3'")*/
		TEXT("NiagaraSystem'/Game/sA_SwordTrailSet/Fx/NS_Star_2.NS_Star_2'")
	);

	if (IsValid(Particle))
		mTrail->SetAsset(Particle);
	mTrail->SetVisibility(true);
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
	PrintViewport(1.f, FColor::Black, TEXT("overlap"));

	const FVector start = mBoxTraceStart->GetComponentLocation();
	const FVector end = mBoxTraceEnd->GetComponentLocation();

	TArray<AActor*> actorsToIgnoreArray;
	actorsToIgnoreArray.Add(this);

	// 한 번에 한 액터를 2번 이상 hit 하지 않도록.
	// ignoreActors 배열은 공격 몽타주가 끝날 때 초기화.
	for (AActor* actor : mIgnoreActors)
		actorsToIgnoreArray.AddUnique(actor);

	FHitResult boxHit;

	UKismetSystemLibrary::BoxTraceSingle(
		this, start, end, FVector(5.f),
		mBoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		actorsToIgnoreArray,	// 추적에서 무시할 액터들의 배열
		EDrawDebugTrace::ForOneFrame,	// None: 디버깅 박스를 보지 않겠다, ForDuration: 몇 초 동안 보겠다.
		boxHit,
		true	// 자기 자신은 무시한다.
	);
	float damage = 10.f;

	// 충돌한 객체가 IHitInterface 를 상속받았을 경우, 해당 클래스의 GetHit() 함수 호출.
	if (boxHit.GetActor())
	{
		//PrintViewport(1.f, FColor::Black, TEXT("overlap!!"));

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

		// AddUnique(): ignoreActors 배열에 넣으려는 액터가 이미 존재하는지 확인.
		mIgnoreActors.AddUnique(boxHit.GetActor());
	}
}

void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

