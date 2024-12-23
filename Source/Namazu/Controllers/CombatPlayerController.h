
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "CombatPlayerController.generated.h"

class UInputAction;

UCLASS()
class NAMAZU_API ACombatPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// Set default values for this player controller 
	ACombatPlayerController();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupInputComponent() override;

private:
	// Components for enhanced input system 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input System", meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input System", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputAction_Move; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input System", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputAction_Look;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input System", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputAction_LookGamepad;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input System", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputAction_Jump;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input System", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputAction_LightAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input System", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputAction_HeavyAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input System", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputAction_LockOn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input System", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputAction_SwitchEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input System", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputAction_CastRope;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input System", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputAction_PrepareReel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input System", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputAction_Grapple;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input System", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputAction_ReelIn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input System", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputAction_InstantAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input System", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputAction_Heal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input System", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputAction_Pause;

	// Reference to player used to call functions
	class APlayerCharacter* Player;

	// General gameplay functions
	void Pause(); 

	// Functions for movement
	void Move(const FInputActionValue& Value); 

	void CallJump(); 

	void CallStopJump(); 

	// Function for camera control
	/** How fast the camera should rotate on gamepad*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera Controls", meta = (AllowPrivateAccess = "true"))
	float GamepadLookRate; 

	void Look(const FInputActionValue& Value);

	void LookGamepad(const FInputActionValue& Value); 

	// Functions for player's combat
	void CallLightAttack();

	void CallHeavyAttack(); 

	void CallInstantAttack(); 

	void CallConsumeMedicine(); 

	// Functions and variable for lock-on system
	void CallLockOn();

	void CallSwitchEnemy(const FInputActionValue& Value); 

	void ResetSwitchEnemy(); 

	bool bCanSwitchEnemy; 

	// Functions for rope 
	void CallCastRope(); 

	void CallGrapple(); 

	void CallReelIn(); 
};
