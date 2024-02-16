// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STPlayersBarWidget.generated.h"

class ASTCharacter;
class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class TESTSHOOTER_API USTPlayersBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	void Init(ASTCharacter* NewCharacter);

protected:
	UFUNCTION()
	void OnCurrentHealthChange(float NewHealth);

	void OnPlayerNameChange(const FString& NewName);
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayerName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> PlayerHPBar;

	// Cached Owner
	UPROPERTY()
	TObjectPtr<ASTCharacter> Character;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor ColorFullHP;
	
	UPROPERTY(EditDefaultsOnly)
	FLinearColor ColorHalfHP;

private:
	float MaxHealth = 0.f;
};
