// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "CommonLib/DynamicValues/CCDynamicValueUtils.h"
#include "Kismet/KismetTextLibrary.h"
#include "ToStringArray.generated.h"

UCLASS()
class FICSITWIREMOD_API AToStringArray : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Out.Empty();
		switch (CurrentStateIndex)
		{
		//Number (Float)
			case 1:
				{
					auto Value = GetConnection(0).GetFloat();
					bool Truncate = GetConnection(1).GetBool();
					if(!Truncate)
					{
						Out.Add(FString::SanitizeFloat(Value));
						break;
					}
				
					int MinFrac = GetConnection(2).GetFloat();
					int MaxFrac = GetConnection(3).GetFloat();

					Out.Add(UKismetTextLibrary::Conv_DoubleToText(
						Value,
						HalfToZero,
						false,
						true,
						1,
						324,
						MinFrac,
						MaxFrac).ToString()
						);
					break;
				}

		default:
			ValueParserCache = UCCDynamicValueUtils::FromValue(GetConnection(0), ValueParserCache);
			if(ValueParserCache) Out = ValueParserCache->ToStringArray();
			break;
		}
	}

	virtual void OnInputConnected_Internal(const FConnectionData& Data, int Index) override
	{
		if(Index == 0 && Data.ConnectionType != StoredType)
		{
			StoredType = Data.ConnectionType;

			//Check if we need to swap the connection list for number formatting
			if(StoredType == ArrayOfNumber) OnStateSelected_Internal(1);
			else OnStateSelected_Internal(0);
		}
		Super::OnInputConnected_Internal(Data, Index);
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AToStringArray, Out)
	}
	
	UPROPERTY(SaveGame)
	TEnumAsByte<EConnectionType> StoredType;
	
	UPROPERTY(Replicated, SaveGame)
	TArray<FString> Out;

	//Utility property so that we don't have to create a new object each time we need to read the value,
	//because the value type will *not* change for 99.9% of the time this gate will function.
	UPROPERTY()
	UCCDynamicValueBase* ValueParserCache;
};
