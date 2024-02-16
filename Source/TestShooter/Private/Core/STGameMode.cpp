// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestShooter/Public/Core/STGameMode.h"

#include "Character/STCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASTGameMode::ASTGameMode()
{
	
}

void ASTGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void ASTGameMode::WaitPlayerRestart(APlayerController* PC)
{
	FTimerDelegate Delegate;
	
	Delegate.BindLambda([this, PC]()
	{
		RestartPlayer(Cast<APlayerController>(PC));
	});
	GetWorld()->GetTimerManager().SetTimer(TimerToRestartPlayer,Delegate, TimeToRestartPlayer, false);
}

void ASTGameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
}
