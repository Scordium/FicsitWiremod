// 

#pragma once

#include "CoreMinimal.h"
#include "ArrayGet.h"
#include "ArrayLength.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "ArrayBreak.generated.h"

UCLASS()
class FICSITWIREMOD_API AArrayBreak : public AFGWiremodBuildable, public IDynamicValuePasser
{
	GENERATED_BODY()
    
public:
	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		ArrayCache = Cast<UCCArrayValueBase>(UCCDynamicValueUtils::FromValue(GetConnection(0), this->GetWorld()));
		if(!ArrayCache) return;
		
		const EConnectionType ElementType = UConnectionTypeFunctions::ArrayToBase(ArrayCache->ConnectionType);
		const int ElementsCount = ArrayCache->Length();

		ConnectionsInfo.Outputs.Empty();
		for(int i = 0; i < ElementsCount; i++)
		{
			ConnectionsInfo.Outputs.Add(
				FBuildingConnection(
					ItemDisplayNameFormat.ToString() + " " + FString::FromInt(i),
					FString::FromInt(i),
					ElementType
					)
				);
		}
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AArrayBreak, ArrayCache)
	}

	virtual void GatherReplicatedObjects_Implementation(TArray<UObject*>& OutObjects) override
	{
		Super::GatherReplicatedObjects_Implementation(OutObjects);
		OutObjects.Add(ArrayCache);
	}

	UPROPERTY(Replicated, SaveGame)
	UCCArrayValueBase* ArrayCache;
	
	UPROPERTY(EditDefaultsOnly)
	FText ItemDisplayNameFormat = FText::FromString("Item");
	
	virtual UObject* GetValue_Implementation(const FString& ValueName) override{ return ArrayCache ? ArrayCache->GetElement(FCString::Atoi(*ValueName)) : nullptr; }
};
