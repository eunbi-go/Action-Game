// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemData.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "../AbilitySystem/AGAttributeSet.h"

UItemData::UItemData()
{
}

UItemData::~UItemData()
{
}

void UItemData::ApplyEffect(ACharacter* target)
{
	UAbilitySystemComponent* targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(target);
	if (targetASC == nullptr)
		return;

	/*
		�����÷��� ȿ���� ȿ�� ���ؽ�Ʈ�� ������ ������ �ִ�.
		�����÷��� ȿ���� �̸� �ѷ��� ���ؽ�Ʈ�� �����ϴ� ������� ���� ����.(ȿ���� ����Ű�� ���ΰ� ���, � ȿ������ ���)
		�����÷��� ȿ�� ���ؽ�Ʈ�� ���� �÷��� ȿ��, Ư�� ȿ�� ���ؽ�Ʈ�� ������ �����Ͽ� ȿ�� ���� ������Ų��.
		������ ȿ�� ���ؽ�Ʈ�� �ڵ��� ó���Ѵ�. ��, �ڵ��� ���ؽ�Ʈ ��ü�� ���� �����͸� �����ϴ� ���̴�.

		Q. ���ؽ�Ʈ �ڵ��� ��� ����°�
		GAS���� ���� �� �ִ�.
		MakeEffectContext() : ȿ�� ���ؽ�Ʈ�� �����, �̿� ���� �ڵ��� ��ȯ.

		��, ����, �� �Ǵ� ������ �����̵� �߰��Ϸ��� ��� ȿ�� ���ؽ�Ʈ�� ���� Ŭ������ �з��Ѵ�.
		�ڵ��� ���� ȿ�� ���ؽ�Ʈ�� �����ͷ� �����ϴ� �淮 ����, ������(data)��� �Ѵ�.
		data�� �����÷��� ȿ�� ���ؽ�Ʈ ������ ���� �������̴�. data == ���� ���ؽ�Ʈ
		�ڵ� == �� ���� ��ƿ��Ƽ�� ���Ե� ����
		���ؽ�Ʈ == ���� �����÷��� ȿ�� ���ؽ�Ʈ�� �����ϴ� ���۸� �����ϴ� ���
	*/
	FGameplayEffectContextHandle ecHandle = targetASC->MakeEffectContext();

	// Source Object : ȿ���� ����
	ecHandle.AddSourceObject(this);

	/*
		1. ��� ASC�� Ư�� Gameplay Effect class�� �־����� Effect Spec�� ���� �� �����Ƿ� ����� ASC�� ����Ѵ�.
		2. Gameplay Effect Level : GameplayEffect�� ������ ������ ���� �� ������ ���� ������Ʈ�� �� Ŀ���� ����
		���� �ٸ� ������ ���� �ȴ�. ������ �׳� 1�� �����Ѵ�.
		Spec Handle : Spec�� �����ϴ� ����
		FGameplayEffectSpecHandle : �������Ʈ�� �����÷��� ȿ�� ������ �� �� �����ϰ� �ڵ�� ������ �� �ֵ��� ����Ѵ�.
									���� ��� � ���� �� �����Ѵ�.
		�ڵ鿡�� ���� ����ü�� �����ϴ� ���� ������ ���(data)�� �ִ�.
	*/
	const FGameplayEffectSpecHandle esHandle = targetASC->MakeOutgoingSpec(mEffect, 1.f, ecHandle);

	const FActiveGameplayEffectHandle agpeHandle = targetASC->ApplyGameplayEffectSpecToSelf(*esHandle.Data.Get());

}
