// Fill out your copyright notice in the Description page of Project Settings.


#include "UMG/GameUI/STWeaponAmmoWidget.h"

#include "Components/TextBlock.h"
#include "Weapon/STBaseWeapon.h"

void USTWeaponAmmoWidget::Init(ASTBaseWeapon* WeaponOwner)
{
	if(WeaponOwner)
	{
		WeaponOwner->OnCurrentAmmoInMagazineChange.BindUObject(this, &USTWeaponAmmoWidget::CurrentAmmoInMagazineChange);
		WeaponOwner->OnCurrentAmmoInBagChange.BindUObject(this, &USTWeaponAmmoWidget::CurrentAmmoInBagChange);
		CurrentAmmoInBagChange(WeaponOwner->GetCurrentAmmoInBag());
		CurrentAmmoInMagazineChange(WeaponOwner->GetCurrentAmmoInMagazine());
	}
}

void USTWeaponAmmoWidget::CurrentAmmoInMagazineChange(int NewAmmo)
{
	CurrentAmmo->SetText(FText::FromString(FString::FromInt(NewAmmo)));
}

void USTWeaponAmmoWidget::CurrentAmmoInBagChange(int NewAmmoInBag)
{
	CurrentAmmoInBag->SetText(FText::FromString(FString::FromInt(NewAmmoInBag)));
}
