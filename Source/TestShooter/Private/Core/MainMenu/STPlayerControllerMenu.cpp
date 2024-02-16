// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MainMenu/STPlayerControllerMenu.h"

#include "UMG/STMenuWidget.h"
#include "Utility/STLogsCategory.h"

void ASTPlayerControllerMenu::BeginPlay()
{
	Super::BeginPlay();

	CreateMenuWidget();

	SetInputMode(FInputModeUIOnly());
	SetShowMouseCursor(true);
}

void ASTPlayerControllerMenu::CreateMenuWidget()
{
	if(!MenuWidgetClass)
	{
		LOG_PRETTY(PCMenuLog, Error, TEXT("Forgot to specify a class for MenuWidgetClass"));
		return;
	}

	MenuWidget = CreateWidget<USTMenuWidget>(this, MenuWidgetClass);
	MenuWidget->AddToViewport();
}
