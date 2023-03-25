#pragma once
#include "CoreMinimal.h"
#include "Configuration/ConfigManager.h"
#include "Engine/Engine.h"
#include "FicsitWiremod_ConfigStruct.generated.h"

/* Struct generated from Mod Configuration Asset '/FicsitWiremod/Other/Structs/FicsitWiremod_Config' */
USTRUCT(BlueprintType)
struct FFicsitWiremod_ConfigStruct {
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite)
    float WiremodWidget_X;

    UPROPERTY(BlueprintReadWrite)
    float WiremodWidget_Y;

    UPROPERTY(BlueprintReadWrite)
    bool WiremodWidget_ShowType;

    UPROPERTY(BlueprintReadWrite)
    float WiremodTool_RaycastDistance;

    UPROPERTY(BlueprintReadWrite)
    int32 WiremodTool_TextSize;

    UPROPERTY(BlueprintReadWrite)
    float Wire_Emission;

    UPROPERTY(BlueprintReadWrite)
    TArray<float> Wire_Color;

    UPROPERTY(BlueprintReadWrite)
    bool Wire_Hidden;

    UPROPERTY(BlueprintReadWrite)
    bool WiremodTool_KeepState;

    UPROPERTY(BlueprintReadWrite)
    float WiremodHolo_SnapSize;

    UPROPERTY(BlueprintReadWrite)
    bool Health_Epilepsy;

    UPROPERTY(BlueprintReadWrite)
    bool HasPassedWelcomingScreen;

    UPROPERTY(BlueprintReadWrite)
    bool WiremodWidget_AlwaysShowDesc;

    UPROPERTY(BlueprintReadWrite)
    bool ConveyorLimiter_DefaultEnabled;

    /* Retrieves active configuration value and returns object of this struct containing it */
    static FFicsitWiremod_ConfigStruct GetActiveConfig() {
        FFicsitWiremod_ConfigStruct ConfigStruct{};
        FConfigId ConfigId{"FicsitWiremod", ""};
        UConfigManager* ConfigManager = GEngine->GetEngineSubsystem<UConfigManager>();
        ConfigManager->FillConfigurationStruct(ConfigId, FDynamicStructInfo{FFicsitWiremod_ConfigStruct::StaticStruct(), &ConfigStruct});
        return ConfigStruct;
    }
};

