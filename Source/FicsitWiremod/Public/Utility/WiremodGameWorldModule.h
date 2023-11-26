// 

#pragma once

#include "CoreMinimal.h"
#include "ConnectionWireBase.h"
#include "FGSchematicManager.h"
#include "Configuration/ConfigManager.h"
#include "ModLoading/ModLoadingLibrary.h"
#include "Module/GameInstanceModuleManager.h"
#include "Module/GameWorldModule.h"
#include "Util/SemVersion.h"
#include "WiremodGameWorldModule.generated.h"


USTRUCT()
struct FModlockedSchematic
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int64 MajorVersion;

	UPROPERTY(EditDefaultsOnly)
	int64 MinorVersion;
	
	UPROPERTY(EditDefaultsOnly)
	int64 Patch;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFGSchematic> Schematic;
};

USTRUCT(BlueprintType)
struct FDeprecatedCircuitryData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AFGBuildable> Class;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText DeprecationText;
};

/**
 * 
 */
UCLASS(BlueprintType)
class FICSITWIREMOD_API UWiremodGameWorldModule : public UGameWorldModule
{
	GENERATED_BODY()
	
public:
	inline static UWiremodGameWorldModule* Self;
	
	virtual void DispatchLifecycleEvent(ELifecyclePhase Phase) override
	{
		Super::DispatchLifecycleEvent(Phase);
		Self = this;
		
		if(Phase == ELifecyclePhase::POST_INITIALIZATION)
		{
			GetWorld()->SpawnActor(DebuggerGimbal, &FTransform::Identity);
			WirePreviewActor = Cast<AConnectionWireBase>(GetWorld()->SpawnActor(AConnectionWireBase::StaticClass(), &FTransform::Identity));
			WirePreviewActor->PreviewMode = true;
			
			RegisterModLockedRecipes();
			SearchForInvalidClasses();
		}
	}

	void RegisterModLockedRecipes()
	{
		auto Player = Cast<AFGCharacterPlayer>(UGameplayStatics::GetPlayerCharacter(this, 0));
		if(!Player || !Player->HasAuthority()) return;
		
		auto ModLib = GetWorld()->GetGameInstance()->GetSubsystem<UModLoadingLibrary>();
		auto SchematicManager = AFGSchematicManager::Get(this);
		TArray<TSubclassOf<UFGSchematic>> AvailableSchematics;
		SchematicManager->GetAllPurchasedSchematics(AvailableSchematics);
		
		for(auto ModRef : ModLockedSchematics)
		{
			FModInfo Mod;
			if(!ModLib->GetLoadedModInfo(ModRef.Key, Mod)) continue;
			if(AvailableSchematics.Contains(ModRef.Value.Schematic)) continue;

			FVersion Version = FVersion(
				ModRef.Value.MajorVersion,
				ModRef.Value.MinorVersion,
				ModRef.Value.Patch
				);
			
			if(!Compare(Version, Mod.Version)) continue;
			
			if(ModRef.Value.Schematic.GetDefaultObject())
			{
				SchematicManager->GiveAccessToSchematic(ModRef.Value.Schematic, nullptr);
			}
		}
	}

	static FORCEINLINE bool Compare(const FVersion& Version, const FVersion& Other)
	{
		return FVersionComparator(EVersionComparisonOp::GREATER_EQUALS, Version).Matches(Other);
	}

	void SearchForInvalidClasses()
	{
		auto Player = UGameplayStatics::GetPlayerCharacter(this, 0);
		if(!Player || !Player->HasAuthority()) return;
		if(DeprecatedClasses.Num() == 0) return;

		TArray<FDeprecatedCircuitryData> Result;
		for(auto DeprecatedData : DeprecatedClasses)
		{
			if(UGameplayStatics::GetActorOfClass(this, DeprecatedData.Class))
			{
				Result.Add(DeprecatedData);
			}
		}

		if(Result.Num() > 0) CreateWiremodDeprecationNotifier(Result);
	}
	
	UFUNCTION(BlueprintImplementableEvent)
	void CreateWiremodDeprecationNotifier(const TArray<FDeprecatedCircuitryData>& Deprecated);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> DebuggerGimbal;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AConnectionWireBase> WirePreviewActorClass;

	UPROPERTY()
	AConnectionWireBase* WirePreviewActor;

protected:
	UPROPERTY(EditDefaultsOnly)
	TMap<FString, FModlockedSchematic> ModLockedSchematics;

	UPROPERTY(EditDefaultsOnly)
	TArray<FDeprecatedCircuitryData> DeprecatedClasses;
};
