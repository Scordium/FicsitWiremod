// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/DynamicValues/CCColorValue.h"
#include "BreakPixelImage.generated.h"

UCLASS()
class FICSITWIREMOD_API ABreakPixelImage : public AFGWiremodBuildable, public IDynamicValuePasser
{
	GENERATED_BODY()

public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		Data = GetConnection(0).GetPixelImage();
	}


	virtual UObject* GetValue_Implementation(const FString& ValueName) override
	{
		auto Index = FCString::Atoi(*ValueName);
		if(Data.Data.IsValidIndex(Index))
		{
			auto Out = NewObject<UCCColorArrayValue>(this);
			Out->Value = Data.Data[Index].RowData;
			return Out;
		}

		return nullptr;
	}


	//We should not replicate this to avoid lag with big images.
	UPROPERTY()
	FPixelScreenData Data;
	
};
