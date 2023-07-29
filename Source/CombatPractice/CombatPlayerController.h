
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "CombatPlayerController.generated.h"

class UInputAction;

UCLASS()
class COMBATPRACTICE_API ACombatPlayerController : public APlayerController
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
	UInputAction* InputAction_Jump;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input System", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputAction_LightAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhanced Input System", meta = (AllowPrivateAccess = "true"))
	UInputAction* InputAction_HeavyAttack;

	// Functions for movement
	void Move(const FInputActionValue& Value); 

	void CallJump(); 

	void CallStopJump(); 

	// Function for camera control
	void Look(const FInputActionValue& Value);

	// Functions for player's attacks
	void CallLightAttack();

	void CallHeavyAttack(); 

	class APlayerCharacter* Player;
};
