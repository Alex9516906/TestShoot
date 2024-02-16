// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TESTSHOOTER_API ASTPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

protected:

	
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void Death();
};
