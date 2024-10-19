#include "ConfigurableConstant.h"

FNamedDynamicValue& AConfigurableConstant::FindValue(FString Name, bool AllowCached)
{
	if(AllowCached && CachedValues.Contains(Name))
		return CachedValues[Name];

	for (FNamedDynamicValue& Value : SavedValues)
	{
		if(Value.GetInternalName().ToString() == Name)
		{
			CachedValues.Add(Name, Value);
			return Value;
		}
		else if(Value.Name == Name)
		{
			CachedValues.Add(Name, Value);
			Value.SetInternalName(Name);
			return Value;
		}
	}
		
	return FallbackValue;
}