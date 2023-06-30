

#pragma once

#include "CoreMinimal.h"
#include "CCImageData.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "PixelArtManager.generated.h"

/**
 * 
 */
UCLASS()
class FICSITWIREMOD_API APixelArtManager : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SetPixelScreenData(const FPixelScreenData& NewData, UObject* Setter)
	{
		if(!GetCanConfigure(Setter)) return;

		Data = NewData;
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(APixelArtManager, Data)
	}
	
	UPROPERTY(Replicated, SaveGame, BlueprintReadWrite)
	FPixelScreenData Data;
};



