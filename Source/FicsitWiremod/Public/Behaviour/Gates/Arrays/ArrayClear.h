// 

#pragma once

#include "CoreMinimal.h"
#include "IConstantsDistributor.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ArrayClear.generated.h"

UCLASS()
class FICSITWIREMOD_API AArrayClear : public AFGWiremodBuildable, public IIConstantsDistributor
{
	GENERATED_BODY()
    
public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		Out.ConnectionType = GetConnection(0).ConnectionType;
		SetOutputType(0, Out.ConnectionType);
	}
    
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
		DOREPLIFETIME(AArrayClear, Out);
	}

	virtual FDynamicValue GetValue_Implementation(const FString& ValueName) override{ return Out; }
	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	FDynamicValue Out;
};
