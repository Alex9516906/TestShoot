// Fill out your copyright notice in the Description page of Project Settings.


#include "TestShooter/Public/Character/STPlayerState.h"

#include "Core/STGameInstance.h"
#include "Net/UnrealNetwork.h"

ASTPlayerState::ASTPlayerState()
{
}

void ASTPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void ASTPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

}
