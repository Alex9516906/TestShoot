// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STWeaponAmmoWidget.generated.h"

class ASTBaseWeapon;
class UTextBlock;
/**
 * 
 */
UCLASS()
class TESTSHOOTER_API USTWeaponAmmoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void Init(ASTBaseWeapon* WeaponOwner);

protected:
	void CurrentAmmoInMagazineChange(int NewAmmo);

	void CurrentAmmoInBagChange(int NewAmmoInBag);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentAmmo;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentAmmoInBag;
};
