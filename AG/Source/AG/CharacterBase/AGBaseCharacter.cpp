
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
	// �� �Լ��� ȣ���ϴ� ��ü ��ü�� Source Object�̴�. 
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
	// Server�� Ability�� �߰��Ѵ�. 

	// false : �ڵ�� Ŭ���̾�Ʈ������ ����ǰ�, ���ʹ� �������� ������ �� 
	// -> Ŭ���̾�Ʈ ����
	// true : �̱� �÷��̰ų�, �����̰ų�, �ش� ���Ͱ� Ŭ���̾�Ʈ���� �����Ѵ�. 
	// -> ���� ����
	if (!HasAuthority())
		return;

	agAbilitySystemComp->AddCharacterAbilities(mStartupAbilites);
}

void AAGBaseCharacter::TryActivateAbility(int32 Id)
{
	auto spec = mAbilitySystemComp->FindAbilitySpecFromInputID(Id);
	if (spec == nullptr)
		return;

	// ��ų ���! (Ability Ȱ��ȭ)
	mAbilitySystemComp->TryActivateAbility(spec->Handle);
}
