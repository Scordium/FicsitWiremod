// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ATan2_Rad.generated.h"

UCLASS()
class FICSITWIREMOD_API AATan2_Rad : public AFGWiremodBuildable
{
	GENERATED_BODY()
    
public:
	virtual void Process_Implementation(float DeltaTime) override
	{
		Out = UKismetMathLibrary::Atan2(GetConnection(0).GetFloat(), GetConnection(1).GetFloat());
	}
    
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
		DOREPLIFETIME(AATan2_Rad, Out);
	}
    
    
	UPROPERTY(Replicated, VisibleInstanceOnly)
	float Out;
};
