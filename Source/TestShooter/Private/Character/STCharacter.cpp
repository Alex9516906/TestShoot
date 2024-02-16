// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/STCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Character/STPlayerController.h"
#include "Character/Components/STStatComponent.h"
#include "Components/WidgetComponent.h"
#include "Core/STGameInstance.h"
#include "Core/STGameMode.h"
#include "Core/STGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UMG/GameUI/STPlayersBarWidget.h"
#include "Weapon/STBaseWeapon.h"


ASTCharacter::ASTCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	
	StatComponent = CreateDefaultSubobject<USTStatComponent>(TEXT("StatComponent"));
	StatComponent->SetIsReplicated(true);

	PlayerStatsWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PlayerStat"));
	PlayerStatsWidgetComponent->SetupAttachment(GetMesh());
	

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ASTCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, FString::Printf(TEXT("%s %s, name %s"),
		HasAuthority() ? TEXT("Server:") : TEXT("Client:"), *GetName(), *GetPlayerName()));
}

void ASTCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	if(!HasAuthority())
	{
		if(GetController())
		{
			PlayerName = Cast<USTGameInstance>(GetGameInstance())->GetPlayerName();
			// Validation is not needed, since the GI is assumed to be always valid. If not, it's better to get a crash
			Server_SetPlayerName(PlayerName);
		}
	}
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	SetupWeapon();

	if(USTPlayersBarWidget* PlayerBar = Cast<USTPlayersBarWidget>(PlayerStatsWidgetComponent->GetWidget()))
	{
		PlayerBar->Init(this);
	}

	StatComponent->OnDeath.AddDynamic(this, &ASTCharacter::Death);
}

void ASTCharacter::OnRepPlayerName()
{
	OnPlayerNameChange.ExecuteIfBound(PlayerName);
}

void ASTCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(HasAuthority())
	{
		if(CurrentWeapon)
		{
			CurrentWeapon->Destroy();
		}
	}
	
	Super::EndPlay(EndPlayReason);
}

void ASTCharacter::Server_SetPlayerName_Implementation(const FString& NewPlayerName)
{
	PlayerName = NewPlayerName;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASTCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Shooting
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ASTCharacter::StartShooting);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &ASTCharacter::StopShooting);

		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &ASTCharacter::Reload);
		
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASTCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASTCharacter::Look);
	}
}

void ASTCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASTCharacter, CurrentWeapon);
	DOREPLIFETIME(ASTCharacter, PlayerName);
}

void ASTCharacter::Death()
{
	if(!HasAuthority())
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetSimulatePhysics(true);
	}
	else
	{
		if(CurrentWeapon)
		{
			CurrentWeapon->Destroy();
			
		}
		if(ASTPlayerController* PC = Cast<ASTPlayerController>(this->GetController()))
		{
			DisableInput(PC);
			if(ASTGameMode* GM = Cast<ASTGameMode>(UGameplayStatics::GetGameMode(this)))
			{
				GM->WaitPlayerRestart(PC);
			}
		}
		SetLifeSpan(3.f);
	}
}

void ASTCharacter::ApplyDamageInterface(float Damage)
{
	StatComponent->SetDamage(Damage);
}

bool ASTCharacter::HasWeapon()
{
	return IsValid(CurrentWeapon);
}

FVector ASTCharacter::GetPositionForIndex() const
{
	FVector Location;
	if(CurrentWeapon && CurrentWeapon->GetMesh())
	{
		Location =  CurrentWeapon->GetMesh()->GetSocketLocation(TEXT("IndexSCT"));
	}
	return Location;
}

void ASTCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASTCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASTCharacter::StartShooting(const FInputActionValue& InputActionValue)
{
	if(CurrentWeapon)
	{
		CurrentWeapon->StartShoot();
	}
}

void ASTCharacter::StopShooting(const FInputActionValue& InputActionValue)
{
	if(CurrentWeapon)
	{
		CurrentWeapon->EndShoot();
	}
}

void ASTCharacter::Reload(const FInputActionValue& InputActionValue)
{
	if(CurrentWeapon)
	{
		CurrentWeapon->ReloadStartWeapon();
	}
}

void ASTCharacter::SetupWeapon()
{
	if(HasAuthority())
	{
		if(WeaponClass)
		{
			CurrentWeapon = GetWorld()->SpawnActor<ASTBaseWeapon>(WeaponClass);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSCT"));
			CurrentWeapon->SetOwner(this);
			CurrentWeapon->SetInstigator(this);
		}
	}
}
