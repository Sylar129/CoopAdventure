// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

#include "MultiplayerSessionsSubsystem.generated.h"

/**
 *
 */
UCLASS()
class COOPADVENTURE_API UMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UMultiplayerSessionsSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	void CreateServer(const FString& ServerName);

	UFUNCTION(BlueprintCallable)
	void FindServer(const FString& ServerName);

	void OnCreateSessionComplete(FName SessionName, bool WasSuccessful);
	void OnDestroySessionComplete(FName SessionName, bool WasSuccessful);
	void OnFindSessionsComplete(bool WasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	IOnlineSessionPtr SessionInterface;

	bool							 CreateServerAfterDestroy;
	FName							 MySessionName;
	FString							 DestroyServerName;
	FString							 ServerNameToFind;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
};
