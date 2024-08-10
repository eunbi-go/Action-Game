// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterInfo.h"

FCharacterClassDefaultInfo UCharacterInfo::GetClassDefaultInfo(ECharacterClass CharacterClass)
{
	return mCharacterInformation.FindChecked(CharacterClass);
}
