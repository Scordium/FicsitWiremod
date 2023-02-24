// 

#pragma once

#include "CoreMinimal.h"
#include "ArrayGet.h"
#include "ArrayLength.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ArrayBreak.generated.h"

UCLASS()
class FICSITWIREMOD_API AArrayBreak : public AFGWiremodBuildable, public IIConstantsDistributor
{
	GENERATED_BODY()
    
public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		const EConnectionType ElementType = UWiremodUtils::ArrayToBase(GetConnection(0).ConnectionType);
		const int ElementsCount = AArrayLength::GetArrayLength(GetConnection(0));

		ConnectionsInfo.Outputs.Empty();
		for(int i = 0; i < ElementsCount; i++)
		{
			ConnectionsInfo.Outputs.Add(
				FBuildingConnection(
					ItemDisplayNameFormat.ToString() + " " + FString::FromInt(i),
					STR::Conv_IntToString(i),
					ElementType
					)
				);
		}
	}

	UPROPERTY(EditDefaultsOnly)
	FText ItemDisplayNameFormat = FText::FromString("Item");
	
	virtual FDynamicValue GetValue_Implementation(const FString& ValueName) override{ return AArrayGet::GetArrayElement(GetConnection(0), FCString::Atoi(*ValueName)); }
};
