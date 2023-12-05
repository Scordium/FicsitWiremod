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
		ArrayCache = Cast<UCCArrayValueBase>(UCCDynamicValueUtils::FromValue(GetConnection(0), ArrayCache));
	}

	virtual TArray<FBuildingConnection> GetConnectionsInfo_Implementation(EConnectionDirection direction, int& Count, FBuildableNote& Note) override
	{
		if(direction == Input) return Super::GetConnectionsInfo_Implementation(direction, Count, Note);
		if(!ArrayCache) return TArray<FBuildingConnection>();

		const EConnectionType ElementType = UConnectionTypeFunctions::ArrayToBase(ArrayCache->ConnectionType);
		const int ElementsCount = ArrayCache->Length();
		TArray<FBuildingConnection> Out;
		for(int i = 0; i < ElementsCount; i++)
		{
			Out.Add(
				FBuildingConnection(
					ItemDisplayNameFormat.ToString() + " " + FString::FromInt(i),
					FString::FromInt(i),
					ElementType
					)
				);
		}

		Count = Out.Num();
		return Out;
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
