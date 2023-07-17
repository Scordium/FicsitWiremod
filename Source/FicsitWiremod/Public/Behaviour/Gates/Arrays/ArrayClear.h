// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/DynamicValues/CCDynamicValueUtils.h"
#include "ArrayClear.generated.h"

UCLASS()
class FICSITWIREMOD_API AArrayClear : public AFGWiremodBuildable, public IDynamicValuePasser
{
	GENERATED_BODY()
    
public:
	virtual void Process_Implementation(double DeltaTime) override
	{
		Out = UCCDynamicValueUtils::FromType(GetConnection(0).ConnectionType, Out ? Out->GetWorld() : this->GetWorld());
		if(auto Array = Cast<UCCArrayValueBase>(Out)) Array->Clear();
		
		SetOutputType(0, Out ? Out->ConnectionType.GetValue() : Unknown);
	}
    
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
		DOREPLIFETIME(AArrayClear, Out);
	}

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override
	{
		bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
		WroteSomething |= Channel->ReplicateSubobject(Out, *Bunch, *RepFlags);

		return WroteSomething;
	}

	virtual UObject* GetValue_Implementation(const FString& ValueName) override{ return Out; }
	
	UPROPERTY(Replicated, VisibleInstanceOnly)
	UCCDynamicValueBase* Out;
};
