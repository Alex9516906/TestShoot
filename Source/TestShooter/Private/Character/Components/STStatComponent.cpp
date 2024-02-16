// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/STStatComponent.h"

#include "Net/UnrealNetwork.h"


USTStatComponent::USTStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTStatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USTStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetAllStats();
}

void USTStatComponent::SetAllStats()
{
	CurrentHealth = MaxHealth;
}

void USTStatComponent::SetDamage(float Damage)
{
	if(GetOwner()->HasAuthority())
	{
		CurrentHealth -=Damage;
		OnCurrentHealthChange.Broadcast(CurrentHealth);
		if(CurrentHealth <= 0)
		{
			Death();
		}
	}
}

void USTStatComponent::Death()
{
	bIsDeath = true;
	OnDeath.Broadcast();
}

void USTStatComponent::OnRep_CurrentHealth()
{
	OnCurrentHealthChange.Broadcast(CurrentHealth);
}

void USTStatComponent::OnRep_IsDeathChange()
{
	OnDeath.Broadcast();
}

void USTStatComponent::Server_SetDamage_Implementation(float Damage)
{
	SetDamage(Damage);
}

void USTStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USTStatComponent, CurrentHealth);
	DOREPLIFETIME(USTStatComponent, bIsDeath);
}

