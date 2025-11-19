// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiplayerSessionsSubsystem.h"

#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Online/OnlineSessionNames.h"

void PrintString(const FString& Str)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, Str);
	}
}

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem()
{
	PrintString("MSS ctor");
	CreateServerAfterDestroy = false;
	DestroyServerName = "";
}

void UMultiplayerSessionsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	PrintString("MSS Initialize");

	PrintString(IOnlineSubsystem::Get()->GetSubsystemName().ToString());

	SessionInterface = IOnlineSubsystem::Get()->GetSessionInterface();
	if (SessionInterface.IsValid())
	{
		PrintString("Session Interface is valid");
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnCreateSessionComplete);
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnDestroySessionComplete);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMultiplayerSessionsSubsystem::OnFindSessionsComplete);
	}
}

void UMultiplayerSessionsSubsystem::Deinitialize()
{
	UE_LOG(LogTemp, Warning, TEXT("MSS Deinitialize"));
}

void UMultiplayerSessionsSubsystem::CreateServer(const FString& ServerName)
{
	PrintString("Create Server");
	if (ServerName.IsEmpty())
	{
		PrintString("Server Name cannot be empty!");
		return;
	}
	FName SessionName("Co-op Adventure Session Name");

	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SessionName);
	if (ExistingSession)
	{
		PrintString("Session already exist, destroying it");
		CreateServerAfterDestroy = true;
		DestroyServerName = ServerName;
		SessionInterface->DestroySession(SessionName);
		return;
	}

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinViaPresence = true;
	SessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";

	SessionInterface->CreateSession(0, SessionName, SessionSettings);
}

void UMultiplayerSessionsSubsystem::FindServer(const FString& ServerName)
{
	PrintString("Find Server");
	if (ServerName.IsEmpty())
	{
		PrintString("Server Name cannot be empty!");
		return;
	}

	SessionSearch = MakeShared<FOnlineSessionSearch>();
	SessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	SessionSearch->MaxSearchResults = 9999;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool WasSuccessful)
{
	PrintString(FString::Printf(TEXT("OnCreateSessionComplete: %d"), WasSuccessful));
	if (WasSuccessful)
	{
		GetWorld()->ServerTravel("/Game/ThirdPerson/Lvl_ThirdPerson?Listen");
	}
}

void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool WasSuccessful)
{
	PrintString(FString::Printf(TEXT("OnDestroySessionComplete: %d"), WasSuccessful));
	if (WasSuccessful && CreateServerAfterDestroy)
	{
		CreateServerAfterDestroy = false;
		CreateServer(DestroyServerName);
	}
}

void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool WasSuccessful)
{
	PrintString(FString::Printf(TEXT("OnFindSessionsComplete: %d"), WasSuccessful));
	if (!WasSuccessful)
	{
		return;
	}
	PrintString(FString::Printf(TEXT("find %d sessions"), SessionSearch->SearchResults.Num()));
}
