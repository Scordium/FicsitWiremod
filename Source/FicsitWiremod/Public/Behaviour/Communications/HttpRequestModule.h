// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HttpModule.h"
#include "Behaviour/FGWiremodBuildable.h"
#include "CommonLib/DynamicValues/CCStringValue.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpRequestModule.generated.h"

UCLASS()
class FICSITWIREMOD_API AHttpRequestModule : public AFGWiremodBuildable
{
	GENERATED_BODY()

public:

	virtual void ServerProcess_Implementation(double DeltaTime) override
	{
		if(GetConnection(0).GetBool())
		{
			if(HasTickedLastFrame) return;

			HasTickedLastFrame = true;

			ResponseString = "";
			ResponseHeaders.Empty();

			auto const Url = GetConnection(1).GetString();
			auto const Content = GetConnection(2).GetString();
			auto const Timeout = GetConnection(3).GetFloat(10);
			auto const Verb = GetConnection(4).GetString("GET");
			auto const Headers = GetConnection(5).GetCustomStruct();

			if(!SupportedVerbs.Contains(Verb.ToUpper()))
			{
				ACircuitryLogger::DispatchErrorEvent("Unsupported CURL verb: " + Verb);
				return;
			}

			auto Request = MakeHttpRequest(Url, Headers, Content, Timeout, Verb);
			CurrentUrl = Url;
			if(!Request->ProcessRequest()) ResponseCode = -1;
			else ResponseCode = 100;
		}
		else HasTickedLastFrame = false;
	}


	FHttpRequestPtr MakeHttpRequest(const FString& Url, const FCustomStruct& Headers, const FString& Content, float Timeout = 10, const FString& Verb = "GET")
	{
		auto Request = FHttpModule::Get().CreateRequest();
		
		Request->SetURL(Url);
		for(auto& Header : Headers.Values)
		{
			if(auto HeaderValue = Cast<UCCStringValue>(Header.Value)) Request->SetHeader(Header.Name, HeaderValue->Value);
		}
		Request->OnRequestProgress().BindUObject(this, &AHttpRequestModule::OnRequestProgressUpdated);
		Request->OnProcessRequestComplete().BindUObject(this, &AHttpRequestModule::OnRequestComplete);
		Request->SetContentAsString(Content);
		Request->SetVerb(Verb);
		Request->SetTimeout(Timeout);

		return Request;
	}

	void OnRequestProgressUpdated(FHttpRequestPtr Request, int Progress1, int Progress2)
	{
		ACircuitryLogger::DispatchEvent("Progress: " + FString::FromInt(Progress1) + " " + FString::FromInt(Progress2), ELogVerbosity::Display);
	}

	void OnRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success)
	{
		ResponseCode = Response->GetResponseCode();
		ResponseString = Response->GetContentAsString();
		ResponseHeaders = Response->GetAllHeaders();
	}

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);

		DOREPLIFETIME(AHttpRequestModule, CurrentUrl)
		DOREPLIFETIME(AHttpRequestModule, ResponseCode)
		DOREPLIFETIME(AHttpRequestModule, ResponseString)
		DOREPLIFETIME(AHttpRequestModule, ResponseHeaders)
	}


	UPROPERTY(Replicated)
	FString CurrentUrl;

	UPROPERTY(Replicated)
	int ResponseCode = -1;

	UPROPERTY(Replicated)
	FString ResponseString;

	UPROPERTY(Replicated)
	TArray<FString> ResponseHeaders;

	UPROPERTY()
	bool HasTickedLastFrame;

	static inline TArray<FString> SupportedVerbs
	{
		"GET",
		"POST",
		"PATCH",
		"PUT",
		"DELETE"
	};
};
