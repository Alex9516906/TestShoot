// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STBaseWeapon.generated.h"

DECLARE_DELEGATE_OneParam(FOnCurrentAmmoInMagazineChange, int32)
DECLARE_DELEGATE_OneParam(FOnCurrentAmmoInBagChange, int32)

USTRUCT(BlueprintType)
struct FWeaponStat
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	float WeaponShootInterval = 1.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	float WeaponDamage = 10.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	float WeaponReloadTime = 3.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	float Distance = 3000.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	int32 MaxAmmo = 50;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	int32 MaxAmmoInMagazine = 10;
};

class UWidgetComponent;

UCLASS()
class TESTSHOOTER_API ASTBaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ASTBaseWeapon();

	void StartShoot();
	void EndShoot();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void Shoot();

	UFUNCTION(Server, Reliable)
	void Server_Shoot(FVector LookDirection);

	// need for VFX
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Shoot(FVector Start, FVector End);

	bool GetLookDirection(APlayerController* PlayerController, FVector& LookDirection);

	void ReloadStartWeapon();

	int32 GetCurrentAmmoInMagazine() const { return CurrentAmmoInMagazine; }
	int32 GetCurrentAmmoInBag() const { return CurrentAmmoInBag; }

	USkeletalMeshComponent* GetMesh();

	// Use this to determine initial weapon settings
	UPROPERTY(EditDefaultsOnly)
	FWeaponStat WeaponStat;

	FOnCurrentAmmoInMagazineChange OnCurrentAmmoInMagazineChange;
	FOnCurrentAmmoInBagChange OnCurrentAmmoInBagChange;
	
	FTimerHandle ShootHandle;
	FTimerHandle ReloadHandle;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_CurrentAmmoInBag();
	
	UFUNCTION()
	void OnRep_CurrentAmmoInMagazine();

	bool CanShoot() const;

	UFUNCTION(Server,Reliable)
	void Server_StartReload();
	
	void ReloadFinish();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> RiffleMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> AmmoWidgetComponent;

	UPROPERTY(Replicated)
	bool bIsReload = false;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentAmmoInMagazine)
	int32 CurrentAmmoInMagazine = 0;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentAmmoInBag)
	int32 CurrentAmmoInBag = 0;
};
