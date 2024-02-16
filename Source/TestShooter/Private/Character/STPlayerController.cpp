// Fill out your copyright notice in the Description page of Project Settings.


#include "TestShooter/Public/Character/STPlayerController.h"
#include "Core/STGameMode.h"
#include "Kismet/GameplayStatics.h"

void ASTPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetShowMouseCursor(false);
	SetInputMode(FInputModeGameOnly());
}

void ASTPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
}

void ASTPlayerController::Death()
{
	if(HasAuthority())
	{
		DisableInput(Cast<ASTPlayerController>(this));
		if(ASTGameMode* GM = Cast<ASTGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			GM->WaitPlayerRestart(this);
		}
	}
}
