// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STPlayerControllerMenu.generated.h"

class USTMenuWidget;
/**
 * 
 */
UCLASS()
class TESTSHOOTER_API ASTPlayerControllerMenu : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	USTMenuWidget* GetMenuWidget() { return MenuWidget; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USTMenuWidget> MenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USTMenuWidget> MenuWidget;

private:
	void CreateMenuWidget();
};
