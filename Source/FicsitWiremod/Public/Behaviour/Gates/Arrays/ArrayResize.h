// 

#pragma once

#include "CoreMinimal.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/DynamicValues/CCDynamicValueUtils.h"
#include "Engine/ActorChannel.h"
#include "ArrayResize.generated.h"

UCLASS()
class FICSITWIREMOD_API AArrayResize : public AFGWiremodBuildable, public IDynamicValuePasser
{
	GENERATED_BODY()
    
public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		const int NewSize = GetConnection(1).GetFloat();
		
		Out = UCCDynamicValueUtils::FromValue(GetConnection(0), Out);
		if(auto Array = Cast<UCCArrayValueBase>(Out)) Array->Resize(NewSize);
		
		SetOutputType(0, Out ? Out->ConnectionType.GetValue() : Unknown);
	}
    
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
		DOREPLIFETIME(AArrayResize, Out);
	}

	virtual void GatherReplicatedObjects_Implementation(TArray<UObject*>& OutObjects) override
	{
		Super::GatherReplicatedObjects_Implementation(OutObjects);
		OutObjects.Add(Out);
	}

	virtual UObject* GetValue_Implementation(const FString& ValueName) override{ return Out; }
    	
	UPROPERTY(Replicated, SaveGame, VisibleInstanceOnly)
	UCCDynamicValueBase* Out;
};
