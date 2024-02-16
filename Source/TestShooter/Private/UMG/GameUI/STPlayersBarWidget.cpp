// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/GameUI/STPlayersBarWidget.h"
#include "Character/STCharacter.h"
#include "Character/Components/STStatComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void USTPlayersBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USTPlayersBarWidget::Init(ASTCharacter* NewCharacter)
{
	Character = NewCharacter;
	if(Character)
	{
		Character->OnPlayerNameChange.BindUObject(this, &USTPlayersBarWidget::OnPlayerNameChange);
		OnPlayerNameChange(Character->GetPlayerName());
		
		if(USTStatComponent* StatComponent = Character->GetStatComponent())
		{
			StatComponent->OnCurrentHealthChange.AddDynamic(this, &USTPlayersBarWidget::OnCurrentHealthChange);
			MaxHealth = StatComponent->GetMaxHealth();
			OnCurrentHealthChange(MaxHealth);
		}
	}
}

void USTPlayersBarWidget::OnPlayerNameChange(const FString& NewName)
{
	PlayerName->SetText(FText::FromString(NewName));
}

void USTPlayersBarWidget::OnCurrentHealthChange(float NewHealth)
{
	PlayerHPBar->SetPercent(NewHealth / MaxHealth);
	if(PlayerHPBar->GetPercent() < 0.5)
	{
		PlayerHPBar->SetFillColorAndOpacity(ColorHalfHP);
	}else
	{
		PlayerHPBar->SetFillColorAndOpacity(ColorFullHP);
	}
}

