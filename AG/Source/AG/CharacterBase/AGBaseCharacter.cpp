
#include "AGBaseCharacter.h"
#include "AbilitySystemComponent.h"

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

void AAGBaseCharacter::InitializeCombatAttributes() const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(mDefaultCombatAttributes);

	const FGameplayEffectContextHandle handle = GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle specHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(mDefaultCombatAttributes, 1.f, handle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*specHandle.Data.Get(), GetAbilitySystemComponent());
}
