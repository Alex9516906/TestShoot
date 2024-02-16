// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/STBaseWeapon.h"

#include "Components/WidgetComponent.h"
#include "Intrerfaces/STDamageInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UMG/GameUI/STWeaponAmmoWidget.h"
#include "Utility/STLogsCategory.h"


ASTBaseWeapon::ASTBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	RiffleMesh = CreateDefaultSubobject<USkeletalMeshComponent>("RiffleMesh");
	RootComponent = RiffleMesh;

	AmmoWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("AmmoWidget");
	AmmoWidgetComponent->SetupAttachment(RootComponent);
}

void ASTBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentAmmoInMagazine = WeaponStat.MaxAmmoInMagazine;
	CurrentAmmoInBag = WeaponStat.MaxAmmo;

	if(AmmoWidgetComponent)
	{
		if(USTWeaponAmmoWidget* AmmoWidget = Cast<USTWeaponAmmoWidget>(AmmoWidgetComponent->GetWidget()))
		{
			AmmoWidget->Init(this);
		}
	}
}

void ASTBaseWeapon::StartShoot()
{
	GetWorld()->GetTimerManager().SetTimer(ShootHandle, this, &ASTBaseWeapon::Shoot, WeaponStat.WeaponShootInterval, true, 0.f);
}

void ASTBaseWeapon::EndShoot()
{
	GetWorld()->GetTimerManager().ClearTimer(ShootHandle);
}

void ASTBaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASTBaseWeapon, CurrentAmmoInMagazine);
	DOREPLIFETIME(ASTBaseWeapon, CurrentAmmoInBag);
}

void ASTBaseWeapon::Shoot()
{
	if(!GetOwner()->HasAuthority())
	{
		FVector LookDirection;
		GetLookDirection(UGameplayStatics::GetPlayerController(this,0), LookDirection);
		Server_Shoot(LookDirection);
	}
}


bool ASTBaseWeapon::GetLookDirection(APlayerController* PlayerController, FVector& LookDirection)
{

	int32 ViewportSizeX, ViewportSizeY;
	PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);
	
	FVector2D ScreenLocation(ViewportSizeX / 2.f, ViewportSizeY / 2.f);
	
	FVector CameraWorldLocation; 
	
	return PlayerController->DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldLocation, LookDirection);
}

void ASTBaseWeapon::Server_Shoot_Implementation(FVector LookDirection)
{
	if(!CanShoot())
	{
		return;
	}
	
	FVector StartLocation = RiffleMesh->GetSocketLocation(TEXT("Muzzle"));
	FVector EndLocation = StartLocation + LookDirection * WeaponStat.Distance;
	FHitResult HitResult;
	UKismetSystemLibrary::LineTraceSingle(this, StartLocation,EndLocation, TraceTypeQuery_MAX, false,
		{this, GetOwner()}, EDrawDebugTrace::ForDuration,HitResult, true);
	
	if(ISTDamageInterface* DamageActor = Cast<ISTDamageInterface>( HitResult.GetActor()))
	{
		DamageActor->ApplyDamageInterface(WeaponStat.WeaponDamage);
	}

	CurrentAmmoInMagazine -= 1;
	Multicast_Shoot(StartLocation, HitResult.TraceEnd);
}


void ASTBaseWeapon::Multicast_Shoot_Implementation(FVector Start, FVector End)
{
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f);
}

void ASTBaseWeapon::OnRep_CurrentAmmoInBag()
{
	OnCurrentAmmoInBagChange.ExecuteIfBound(CurrentAmmoInBag);
}

void ASTBaseWeapon::OnRep_CurrentAmmoInMagazine()
{
	OnCurrentAmmoInMagazineChange.ExecuteIfBound(CurrentAmmoInMagazine);
}

bool ASTBaseWeapon::CanShoot() const
{
	return CurrentAmmoInMagazine > 0 && !bIsReload;
}

void ASTBaseWeapon::Server_StartReload_Implementation()
{
	if(!GetWorld()->GetTimerManager().TimerExists(ReloadHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(ReloadHandle, this, &ASTBaseWeapon::ReloadFinish,
		WeaponStat.WeaponReloadTime, false, -1);
	}
}

void ASTBaseWeapon::ReloadStartWeapon()
{
	if(CurrentAmmoInBag > 0)
	{
		Server_StartReload();
	}
	// Set this animation
}

USkeletalMeshComponent* ASTBaseWeapon::GetMesh()
{
	return RiffleMesh;
}

void ASTBaseWeapon::ReloadFinish()
{
	if(CurrentAmmoInBag > 0)
	{
		int32 NeedAmmo = WeaponStat.MaxAmmoInMagazine - CurrentAmmoInMagazine;
		if (CurrentAmmoInBag >= NeedAmmo)
		{
			CurrentAmmoInMagazine += NeedAmmo;
			CurrentAmmoInBag -= NeedAmmo;
			
		} else
		{
			CurrentAmmoInMagazine += CurrentAmmoInBag;
			CurrentAmmoInBag = 0;
		}
	}
	GetWorld()->GetTimerManager().ClearTimer(ReloadHandle);
}

