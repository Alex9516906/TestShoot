// Fill out your copyright notice in the Description page of Project Settings.


#include "ExternalActor/STTriggerDamage.h"

#include "Intrerfaces/STDamageInterface.h"


// Sets default values
ASTTriggerDamage::ASTTriggerDamage()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASTTriggerDamage::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ASTTriggerDamage::BeginOverlap);
}

// Called every frame
void ASTTriggerDamage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASTTriggerDamage::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if(ISTDamageInterface* DamageActor = Cast<ISTDamageInterface>( OtherActor))
	{
		DamageActor->ApplyDamageInterface(Damage);
	}
}

