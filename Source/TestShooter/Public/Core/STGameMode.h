// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "STGameMode.generated.h"

UCLASS(minimalapi)
class ASTGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASTGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	void WaitPlayerRestart(APlayerController* PC);

	virtual void RestartPlayer(AController* NewPlayer) override;

protected:
	FTimerHandle TimerToRestartPlayer;
	float TimeToRestartPlayer = 3.5f;
};



