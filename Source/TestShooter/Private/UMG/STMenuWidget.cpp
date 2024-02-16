// Fill out your copyright notice in the Description page of Project Settings.


#include "TestShooter/Public/UMG/STMenuWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Core/STGameInstance.h"
#include "Utility/STLogsCategory.h"

void USTMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if(IPAddress)
	{
		IPAddress->SetText(FText::FromString(TEXT("127.0.0.1:7777")));
	}
	if(NamePlayer)
	{
		NamePlayer->SetHintText(FText::FromString(TEXT("Player")));
	}
	if(ConnectButton)
	{
		ConnectButton->OnClicked.AddDynamic(this, &USTMenuWidget::OnConnectButtonClick);
	}
}

void USTMenuWidget::OnConnectButtonClick()
{
	if(NamePlayer && NamePlayer->GetText().IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Please enter player name"));
		return;
	}
	
	USTGameInstance* GI = Cast<USTGameInstance>(GetGameInstance());
	
	// No verification is needed at this point. If our GameInstance is not valid, it’s better to crash
	check(GI);
	GI->SaveNameInGameInstance(NamePlayer->GetText().ToString());
	GI->ConnectToServer(GetIPAddress());
}

FName USTMenuWidget::GetIPAddress()
{
	if(!IPAddress)
	{
		return FName();
	}
	return  FName(IPAddress->GetText().ToString());
}
