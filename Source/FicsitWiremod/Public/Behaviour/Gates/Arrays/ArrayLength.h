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
		const auto Conn = GetConnection(0);
		const auto Value = UCCDynamicValueUtils::FromValue(Conn, Conn.Object);
		
		if(auto Array = Cast<UCCArrayValueBase>(Value))
			Out = Array->Length();
		else Out = 0;
	}
    
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
		DOREPLIFETIME(AArrayLength, Out);
	}
	
	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	int Out;
};
