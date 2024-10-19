#include "CustomStruct.h"

FCustomStruct FCustomStruct::Duplicate(UObject* WorldContext)
{
	auto NewValues = TArray<FNamedDynamicValue>();

	for(int i = 0; i < Values.Num(); i++)
	{
		auto OldValue = Values[i].Value;
		if (OldValue != nullptr)
		{
			auto NewValue = UCCDynamicValueUtils::FromWrapper(WorldContext, OldValue->ToWrapperValue());
			NewValues.Add(FNamedDynamicValue(Values[i].Name, NewValue));
		}
	}

	return FCustomStruct(Name, NewValues);
}