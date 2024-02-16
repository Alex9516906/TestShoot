// Fill out your copyright notice in the Description page of Project Settings.


#include "TestShooter/Public/Core/STGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/STLogsCategory.h"

void USTGameInstance::ConnectToServer(FName Address) const
{
	UGameplayStatics::OpenLevel(this, Address);
}

void USTGameInstance::SaveNameInGameInstance(const FString& NewName)
{
	PlayerName = NewName;
}
