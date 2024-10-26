// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemodes/LobbyGameMode.h"

#include "GameFramework/GameStateBase.h"

void ALobbyGameMode::ServerSeamlessTravel(const FString& LevelPath)
{
	if(UWorld* World = GetWorld())
	{
		bUseSeamlessTravel = true;
		World->ServerTravel(FString(LevelPath + "?listen"));
	}
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int32 NumOfPlayer = GameState.Get()->PlayerArray.Num();
	if(NumOfPlayer == 2)
	{
		ServerSeamlessTravel("/Game/Maps/TestShooterMap");
	}
}

