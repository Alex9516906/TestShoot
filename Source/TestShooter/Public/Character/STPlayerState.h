// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "STPlayerState.generated.h"


/**
 * 
 */
UCLASS()
class TESTSHOOTER_API ASTPlayerState : public APlayerState
{
	GENERATED_BODY()
public:

protected:
	ASTPlayerState();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	
};
