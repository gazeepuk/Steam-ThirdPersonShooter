// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class STEAMTPS_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void ServerSeamlessTravel(const FString& LevelPath);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString DefaultLevelToTravelPath = "/Game/Maps/TestShooterMap";
protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
};