#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
#include "Intrerfaces/STDamageInterface.h"
#include "Logging/LogMacros.h"
#include "STCharacter.generated.h"

class ASTBaseWeapon;
class UWidgetComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class USTStatComponent;
struct FInputActionValue;

DECLARE_DELEGATE_OneParam(FOnPlayerNameChange, const FString&);

UCLASS(config=Game)
class ASTCharacter : public ACharacter, public ISTDamageInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USTStatComponent> StatComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> PlayerStatsWidgetComponent;
	
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Shoot Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ShootAction;

	/** Reload Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

public:
	ASTCharacter();

	virtual void Tick(float DeltaSeconds) override;
	// ISTDamageInterface
	virtual void ApplyDamageInterface(float Damage) override;
	// end ISTDamageInterface

	USTStatComponent* GetStatComponent() const { return StatComponent; }

	UFUNCTION(BlueprintPure)
	bool HasWeapon();

	// Obtaining these coordinates is necessary for fine-tuning IK, but due to the fast
	// development speed, I skip this feature. At the moment, just transforming the bone into anime bp
	UFUNCTION(BlueprintPure)
	FVector GetPositionForIndex() const;
	
	FOnPlayerNameChange OnPlayerNameChange;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void StartShooting(const FInputActionValue& InputActionValue);
	void StopShooting(const FInputActionValue& InputActionValue);

	// reload weapon
	void Reload(const FInputActionValue& InputActionValue);

	// init setup weapon
	void SetupWeapon();
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Handling player death
	UFUNCTION()
	void Death();
	
	// To add mapping context
	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
	TObjectPtr<ASTBaseWeapon> CurrentWeapon = nullptr;

	// It’s better to do this through DataAsset as a way to select weapons, but since time is limited, we’ll do it through TSubClassOf
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ASTBaseWeapon> WeaponClass;

	UPROPERTY(ReplicatedUsing = OnRepPlayerName)
	FString PlayerName = "TestPlayer";

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FString& GetPlayerName() { return PlayerName; }

	UFUNCTION()
	void OnRepPlayerName();

	UFUNCTION(Server, Reliable)
	void Server_SetPlayerName(const FString& NewPlayerName);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};