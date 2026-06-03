#include "Utility/WikiParser.h"

#include "Behaviour/FGWiremodBuildable.h"
#include "JsonObjectConverter.h"


bool UCircuitryWikiParser::SaveAsset(const FModuleDocumentation& Document)
{
	const FString BaseFilePath = FPaths::ProjectSavedDir() + "CIRCUITRY_WIKIPARSE/";
	ACircuitryLogger::DispatchEvent("Base path is " + BaseFilePath, ELogVerbosity::Display);
	
	const FString FileName = Document.ClassName + ".json";

	TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(Document);
	if(!JsonObject)
	{
		ACircuitryLogger::DispatchErrorEvent("Failed to create json object for class " + Document.ClassName);
		return false;
	}

	FString FileText;
	if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), TJsonWriterFactory<>::Create(&FileText, 0)))
	{
		ACircuitryLogger::DispatchErrorEvent("Failed to serialize json object for class " + Document.ClassName);
		return false;
	}
		
	FFileHelper::SaveStringToFile(FileText, *(BaseFilePath + FileName));

	ACircuitryLogger::DispatchEvent("Writing json to file " + FileName, ELogVerbosity::Display);
	return true;
}
