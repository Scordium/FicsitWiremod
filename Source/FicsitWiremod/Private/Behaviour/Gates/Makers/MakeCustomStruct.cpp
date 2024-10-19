#include "MakeCustomStruct.h"

void ACustomStructProcessor::SetCustomStruct_Internal(const FCustomStruct& NewStruct)
{
	//Disconnect all changed inputs
	for(int i = 0; i < NewStruct.Values.Num(); i++)
	{
		if(i >= Out.Values.Num()) break;

		auto& OldValue = Out.Values[i];
		auto& NewValue = NewStruct.Values[i];
			
		if(!NewValue.Value || OldValue.Value->ConnectionType != NewValue.Value->ConnectionType || OldValue.Name != NewValue.Name)
			OnInputDisconnected_Internal(i);
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