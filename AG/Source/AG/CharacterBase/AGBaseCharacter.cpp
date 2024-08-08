
#include "AGBaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "../AbilitySystem/AGAbilitySystemComponent.h"

AAGBaseCharacter::AAGBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

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

void AAGBaseCharacter::Death()
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

	// false : 코드는 클라이언트에서만 실행되고, 액터는 서버에서 복제된 것 
	// -> 클라이언트 권한
	// true : 싱글 플레이거나, 서버이거나, 해당 액터가 클라이언트에만 존재한다. 
	// -> 서버 권한
	if (!HasAuthority())
		return;

	agAbilitySystemComp->AddCharacterAbilities(mStartupAbilites);
}

void AAGBaseCharacter::TryActivateAbility(int32 Id)
{
	auto spec = mAbilitySystemComp->FindAbilitySpecFromInputID(Id);
	if (spec == nullptr)
		return;

	// 스킬 사용! (Ability 활성화)
	mAbilitySystemComp->TryActivateAbility(spec->Handle);
}
