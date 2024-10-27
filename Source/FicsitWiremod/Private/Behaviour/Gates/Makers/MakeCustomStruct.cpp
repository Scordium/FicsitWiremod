#include "MakeCustomStruct.h"

void ACustomStructProcessor::SetCustomStruct_Internal(const FCustomStruct& NewStruct)
{
	//Disconnect all changed inputs
	for(int i = 0; i < NewStruct.Values.Num() && i < Out.Values.Num(); i++)
	{
		const auto& OldValue = Out.Values[i];
		const auto& NewValue = NewStruct.Values[i];
		
		if(OldValue.IsDataTypeEqual(NewValue) == false) OnInputDisconnected_Internal(i);
	}

	//Disconnect all stale inputs that remain in case the new struct is smaller.
	if(Out.Values.Num() > NewStruct.Values.Num())
	{
		for(int i = Out.Values.Num() - 1; i >= NewStruct.Values.Num(); i--)
		{
			OnInputDisconnected_Internal(i);
		}
	}
		
	Out = NewStruct;
	Recompile();
}