// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "STTriggerDamage.generated.h"

UCLASS()
class TESTSHOOTER_API ASTTriggerDamage : public ATriggerBox
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASTTriggerDamage();

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void BeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 30;
};
