
#include "AttributeInfo.h"

FAGAttributeInfo UAttributeInfo::FindAttributeInfoByTag(const FGameplayTag& AttributeTag)
{
	for (const FAGAttributeInfo& info : mAttributeInfosArray)
	{
		// ��Ȯ�� ��ġ�ϴ��� Ȯ���Ѵ�.
		if (info.mAttributeTag.MatchesTagExact(AttributeTag))
		{
			return info;
		}
	}

	return FAGAttributeInfo();
}
