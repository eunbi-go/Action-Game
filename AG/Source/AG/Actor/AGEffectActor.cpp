// Fill out your copyright notice in the Description page of Project Settings.


#include "AGEffectActor.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "../AbilitySystem/AGAttributeSet.h"

AAGEffectActor::AAGEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(mMesh);

	mSphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	mSphere->SetupAttachment(GetRootComponent());
}

void AAGEffectActor::BeginPlay()
{
	Super::BeginPlay();
	mSphere->OnComponentBeginOverlap.AddDynamic(this, &AAGEffectActor::OnOverlap);
	mSphere->OnComponentEndOverlap.AddDynamic(this, &AAGEffectActor::EndOverlap);
}

void AAGEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAGEffectActor::OnOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 다른 액터의 능력 시스템 구성 요소에 접근한다.
	// 효과 액터는 다양한 방법으로 모든 속성에 모든 효과를 적용할 수 있어야 한다.
	// 다재다능하고 재사용이 가능해야 하며, 포인터의 불변성을 위반해서는 안된다.
	if (IAbilitySystemInterface* interface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UAGAttributeSet* attributeSet = Cast<UAGAttributeSet>(interface->GetAbilitySystemComponent()->GetAttributeSet(UAGAttributeSet::StaticClass()));
		UAGAttributeSet* nas = const_cast<UAGAttributeSet*>(attributeSet);
		nas->SetmHp(attributeSet->GetmHp() + 25.f);
		Destroy();
	}
}
void AAGEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

