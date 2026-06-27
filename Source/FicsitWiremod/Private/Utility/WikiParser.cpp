#include "Utility/WikiParser.h"

#include "Behaviour/FGWiremodBuildable.h"
#include "JsonObjectConverter.h"
#include "CommonLib/JsonUtilities.h"

void UCircuitryWikiParser::SaveAssets(TMap<AFGWiremodBuildable*, UTexture2D*> Buildables)
{
#if WITH_EDITOR
	TArray<TSharedPtr<FJsonValue>> Array;
	for (auto Buildable : Buildables)
	{
		auto ModuleDocument = Buildable.Key->GenerateWikiModuleFile(Buildable.Value);
		auto Object = MakeShareable(new FJsonValueObject(FJsonObjectConverter::UStructToJsonObject(ModuleDocument)));
		
		Array.Add(Object);
	}
	
	FString JsonString;
	if (!FJsonSerializer::Serialize(Array, TJsonWriterFactory<>::Create(&JsonString, 0)))
		ACircuitryLogger::DispatchErrorEvent("There was an error creating json string");
	
	const FString BaseFilePath = FString(FPlatformProcess::UserHomeDir()) + "/CircuitryWikiParse.json";
	ACircuitryLogger::DispatchEvent("Writing json to file " + BaseFilePath, ELogVerbosity::Display);
	
	if (FFileHelper::SaveStringToFile(JsonString, *BaseFilePath)) 
		ACircuitryLogger::DispatchEvent("Saved " + FString::FromInt(Array.Num()) + " entries to file.", ELogVerbosity::Display);
	else 
		ACircuitryLogger::DispatchErrorEvent("There was an error saving the json string");
#else
	ACircuitryLogger::DispatchErrorEvent("Attempted to call wiki parser at runtime");
#endif
}
