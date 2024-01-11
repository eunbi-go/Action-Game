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
	// �ٸ� ������ �ɷ� �ý��� ���� ��ҿ� �����Ѵ�.
	// ȿ�� ���ʹ� �پ��� ������� ��� �Ӽ��� ��� ȿ���� ������ �� �־�� �Ѵ�.
	// ����ٴ��ϰ� ������ �����ؾ� �ϸ�, �������� �Һ����� �����ؼ��� �ȵȴ�.
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

