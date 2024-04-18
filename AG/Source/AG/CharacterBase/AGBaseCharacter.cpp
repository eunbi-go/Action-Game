
#include "AGBaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "../AbilitySystem/AGAbilitySystemComponent.h"

AAGBaseCharacter::AAGBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	//mWeapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	//mWeapon->SetupAttachment(GetMesh(), FName("UnEquipSword"));
	//mWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAGBaseCharacter::GetAbilitySystemComponent() const
{
	return mAbilitySystemComp;
}

void AAGBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAGBaseCharacter::InitAbilityActorInfo()
{
}

void AAGBaseCharacter::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);

	FGameplayEffectContextHandle handle = GetAbilitySystemComponent()->MakeEffectContext();
	// 이 함수를 호출하는 객체 자체가 Source Object이다. 
	handle.AddSourceObject(this);
	const FGameplayEffectSpecHandle specHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, handle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*specHandle.Data.Get(), GetAbilitySystemComponent());
}

void AAGBaseCharacter::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(mDefaultFirstAttributes, 1.f);
	ApplyEffectToSelf(mDefaultSecondAttributes, 1.f);
	ApplyEffectToSelf(mDefaultVitalAttributes, 1.f);
}

void AAGBaseCharacter::AddCharacterAbilities()
{
	UAGAbilitySystemComponent* agAbilitySystemComp = CastChecked<UAGAbilitySystemComponent>(mAbilitySystemComp);
	// Server에 Ability를 추가한다. 

	if (!HasAuthority())
		return;

	agAbilitySystemComp->AddCharacterAbilities(mStartupAbilites);
}
