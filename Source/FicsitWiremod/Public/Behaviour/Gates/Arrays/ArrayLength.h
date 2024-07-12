// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ArrayLength.generated.h"

UCLASS()
class FICSITWIREMOD_API AArrayLength : public AFGWiremodBuildable
{
	GENERATED_BODY()
    
public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		ArrayCache = UCCDynamicValueUtils::FromValue(GetConnection(0), ArrayCache);
		
		if(auto Array = Cast<UCCArrayValueBase>(ArrayCache))
			Out = Array->Length();
		else Out = 0;
	}
    
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
		DOREPLIFETIME(AArrayLength, Out);
	}

	UPROPERTY()
	UCCDynamicValueBase* ArrayCache;
	
	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	int Out;
};
