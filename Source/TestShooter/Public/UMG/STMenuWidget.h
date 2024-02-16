// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STMenuWidget.generated.h"

class UEditableText;
class UButton;

/**
 * 
 */
UCLASS()
class TESTSHOOTER_API USTMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
protected:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnConnectButtonClick();

private:
	FName GetIPAddress();
	
	UPROPERTY(EditAnywhere, Category = BaseWidget, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> ConnectButton;
	
	UPROPERTY(EditAnywhere, Category = BaseWidget, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UEditableText> IPAddress;

	UPROPERTY(EditAnywhere, Category = BaseWidget, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UEditableText> NamePlayer;
};
