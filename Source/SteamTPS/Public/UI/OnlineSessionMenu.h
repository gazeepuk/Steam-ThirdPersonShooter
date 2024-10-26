// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionMenu.generated.h"

/**
 * 
 */
UCLASS()
class STEAMTPS_API UOnlineSessionMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup(int32 NumberOfPublicConnections = 4, FString TypeOfMatch = FString(TEXT("FreeForAll")), FString LobbyPath = FString(TEXT("/Game/Maps/Lobbies/LobbyTestLevel")));

protected:
	
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;

	//
	// Callbacks for the custom delegates on the MultiplayerSessionsSubsystem
	//
	UFUNCTION()
	virtual void OnCreateSession(bool bWasSuccessful);
	virtual void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION()
	virtual void OnDestroySession(bool bWasSuccessful);
	UFUNCTION()
	virtual void OnStartSession(bool bWasSuccessful);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn))
	int32 NumPublicConnections{4};
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn))
	FString MatchType{TEXT("FreeForAll")};
private:

	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	UFUNCTION()
	void HostButtonClicked();

	UFUNCTION()
	void JoinButtonClicked();

	void MenuTearDown();

	// The subsystem designed to handle all online session functionality
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	FString PathToLobby{TEXT("")};
};
