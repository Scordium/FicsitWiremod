// Copyright Coffee Stain Studios. All Rights Reserved.


#include "WiremodRemoteCallObject.h"
#include "Util/BlueprintLoggingLibrary.h"

void UWiremodRemoteCallObject::DoSomething_Implementation(const FString& SomeData)
{
	UBlueprintLoggingLibrary::LogDisplay(SomeData);
}


bool UWiremodRemoteCallObject::DoSomething_Validate(const FString& SomeData)
{
	return true;
}


void UWiremodRemoteCallObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UWiremodRemoteCallObject, bDummy);
}
