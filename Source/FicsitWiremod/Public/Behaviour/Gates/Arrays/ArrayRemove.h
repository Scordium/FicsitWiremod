// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/DynamicValues/CCDynamicValueUtils.h"
#include "ArrayRemove.generated.h"

UCLASS()
class FICSITWIREMOD_API AArrayRemove : public AFGWiremodBuildable, public IDynamicValuePasser
{
	GENERATED_BODY()
    
public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		Out = UCCDynamicValueUtils::FromValue(GetConnection(0), Out ? Out->GetWorld() : this->GetWorld());
		const int Index = GetConnection(1).GetFloat();
		if(auto Array = Cast<UCCArrayValueBase>(Out)) Array->RemoveElement(Index);
		
		SetOutputType(0, Out ? Out->ConnectionType.GetValue() : Unknown);
	}
    
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
		DOREPLIFETIME(AArrayRemove, Out);
	}

	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override
	{
		bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
		WroteSomething |= Channel->ReplicateSubobject(Out, *Bunch, *RepFlags);

		return WroteSomething;
	}

	virtual UObject* GetValue_Implementation(const FString& ValueName) override{ return Out; }
    	
	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	UCCDynamicValueBase* Out;
};
