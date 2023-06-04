// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "Behaviour/MultistateWiremodBuildable.h"
#include "Kismet/KismetTextLibrary.h"
#include "ToString.generated.h"

UCLASS()
class FICSITWIREMOD_API AToString : public AMultistateWiremodBuildable
{
	GENERATED_BODY()

public:

	virtual void Process_Implementation(float DeltaTime) override
	{
		switch (CurrentStateIndex)
		{
			//Number (Float)
		case 1:
			{
				float Value = WM_GetFloat(0);
				bool Truncate = WM_GetBool(1);
				if(!Truncate)
				{
					Out = FString::SanitizeFloat(Value);
					break;
				}
				
				int MinFrac = WM_GetInt(2);
				int MaxFrac = WM_GetInt(3);

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
				Out = UWiremodUtils::GetStringifiedValue(GetConnection(0));
			break;
		}
	}

	virtual void OnInputConnected_Internal(const FNewConnectionData& Data, int Index) override
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
};
