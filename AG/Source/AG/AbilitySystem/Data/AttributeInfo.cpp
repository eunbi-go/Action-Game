
#include "AttributeInfo.h"

FAGAttributeInfo UAttributeInfo::FindAttributeInfoByTag(const FGameplayTag& AttributeTag)
{
	for (const FAGAttributeInfo& info : mAttributeInfosArray)
	{
		// 정확히 일치하는지 확인한다.
		if (info.mAttributeTag.MatchesTagExact(AttributeTag))
		{
			return info;
		}
	}

	return FAGAttributeInfo();
}
