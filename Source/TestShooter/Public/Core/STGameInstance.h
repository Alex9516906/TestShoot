// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "STGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TESTSHOOTER_API USTGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// Get player name from saved
	FString& GetPlayerName() { return PlayerName; }
	
	// Saving the player's name to a variable
	void SaveNameInGameInstance(const FString& NewName);

	// OpenMap is used internally
	void ConnectToServer(FName Address) const;

private:
	FString PlayerName = "None";
};
