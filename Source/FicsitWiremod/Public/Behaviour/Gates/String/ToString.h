// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "CommonLib/DynamicValues/CCDynamicValueUtils.h"
#include "Kismet/KismetTextLibrary.h"
#include "ToString.generated.h"

UCLASS()
class FICSITWIREMOD_API AToString : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:

	virtual void Process_Implementation(double DeltaTime) override
	{
		switch (CurrentStateIndex)
		{
			//Number (Float)
		case 1:
			{
				double Value = GetConnection(0).GetFloat();
				bool Truncate = GetConnection(1).GetBool();
				if(!Truncate)
				{
					Out = FString::SanitizeFloat(Value);
					break;
				}
				
				int MinFrac = GetConnection(2).GetFloat();
				int MaxFrac = GetConnection(3).GetFloat();

				Out = UKismetTextLibrary::Conv_FloatToText(
					Value,
					HalfToZero,
					false,
					true,
					1,
					324,
					MinFrac,
					MaxFrac).ToString();
				break;
			}

		default:
			ValueParserCache = UCCDynamicValueUtils::FromValue(GetConnection(0), ValueParserCache ? ValueParserCache->GetWorld() : this->GetWorld());
			if(ValueParserCache) Out = ValueParserCache->ToString();
			else Out = "";
			break;
		}
	}

	virtual void OnInputConnected_Internal(const FConnectionData& Data, int Index) override
	{
		if(Index == 0 && Data.ConnectionType != StoredType)
		{
			StoredType = Data.ConnectionType;

			//Check if we need to swap the connection list for number formatting
			if(StoredType == Number) OnStateSelected_Internal(1);
			else OnStateSelected_Internal(0);
		}
		Super::OnInputConnected_Internal(Data, Index);
	}


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AToString, Out)
	}
	
	UPROPERTY(SaveGame)
	TEnumAsByte<EConnectionType> StoredType;
	
	UPROPERTY(Replicated)
	FString Out;


	//Utility property so that we don't have to create a new object each time we need to read the value,
	//because the value type will *not* change for 99.9% of the time this gate will function.
	UPROPERTY()
	UCCDynamicValueBase* ValueParserCache;
};
