// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STStatComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentHealthChange, float, NewHealth);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TESTSHOOTER_API USTStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USTStatComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	float GetCurrentHealth() const { return CurrentHealth; }

	float GetMaxHealth() const { return MaxHealth; }

	void SetDamage(float Damage);

	bool IsDeath() const { return bIsDeath; }

	void Death();

	UFUNCTION()
	void OnRep_CurrentHealth();

	UFUNCTION()
	void OnRep_IsDeathChange();

	FOnDeath OnDeath;

	FOnCurrentHealthChange OnCurrentHealthChange;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	void SetAllStats();

	UFUNCTION(Server, Reliable)
	void Server_SetDamage(float Damage);

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	float CurrentHealth = 0.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = Stats)
	float MaxHealth = 100.f;

	// It’s better to replace states with gameplay tags.
	UPROPERTY(ReplicatedUsing = OnRep_IsDeathChange)
	bool bIsDeath = false;

};
