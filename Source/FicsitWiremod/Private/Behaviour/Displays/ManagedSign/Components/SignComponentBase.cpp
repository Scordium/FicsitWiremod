#include "Behaviour/Displays/ManagedSign/Components/SignComponentBase.h"

#include "Behaviour/Displays/ManagedSign/ManagedSign.h"


void FVariableValueBindingData::Call(AFGWiremodBuildable* Sign)
{
	if(InputIndex == -1)
	{
		if(BindingName.ToString().IsNumeric())
		{
			InputIndex = FCString::Atoi(*BindingName.ToString());
		}
		else
		{
			auto SignCast = Cast<AManagedSign>(Sign);
			if(!SignCast)
			{
				ACircuitryLogger::DispatchErrorEvent("Failed to cast sign entity, cancelling binding call. This should NEVER happen.");
				return;
			}
	
			for(int i = 0; i < SignCast->Data.Connections.Num(); i++)
			{
				auto& Binding = SignCast->Data.Connections[i];
				if(Binding.InternalName == BindingName)
				{
					InputIndex = i;
					break;
				}
			}
		}
	}
	
	if(Sign) Event.ExecuteIfBound(Sign->GetConnection(InputIndex), DefaultValue, MetaData);
}
