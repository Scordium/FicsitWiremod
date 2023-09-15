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
		Out = GetArrayLength(GetConnection(0));
	}

	static int GetArrayLength(const FConnectionData& Data)
	{
		auto Value = UCCDynamicValueUtils::FromValue(Data, Data.Object);
		if(auto Array = Cast<UCCArrayValueBase>(Value))
			return  Array->Length();

		return 0;
	}
    
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
		DOREPLIFETIME(AArrayLength, Out);
	}
	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	int Out;
};
