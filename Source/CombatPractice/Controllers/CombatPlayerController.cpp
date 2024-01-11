
#include "CombatPlayerController.h"
#include "CombatPractice/Characters/PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACombatPlayerController::ACombatPlayerController()
{
	InputMappingContext = nullptr;
	InputAction_Move = nullptr; 
	InputAction_Look = nullptr; 
	InputAction_Jump = nullptr; 
	InputAction_LightAttack = nullptr; 
	InputAction_HeavyAttack = nullptr; 
	InputAction_LockOn = nullptr;
	InputAction_SwitchEnemyUp = nullptr; 
}

// Called when the game starts
void ACombatPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeGameOnly());

	Player = Cast<APlayerCharacter>(GetPawn());
	if (Player == nullptr)
		return; 
}

// Called to bind functionality to input
void ACombatPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	// Add mapping context to controller
	UEnhancedInputLocalPlayerSubsystem* LocalSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer());
	if (LocalSubsystem)
	{
		LocalSubsystem->ClearAllMappings();
		LocalSubsystem->AddMappingContext(InputMappingContext, 0);
	}

	// Bind actions to input system
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// MOVEMENT
		EnhancedInput->BindAction(InputAction_Move, ETriggerEvent::Triggered, this, &ACombatPlayerController::Move);
		EnhancedInput->BindAction(InputAction_Jump, ETriggerEvent::Ongoing, this, &ACombatPlayerController::CallJump);
		EnhancedInput->BindAction(InputAction_Jump, ETriggerEvent::Canceled, this, &ACombatPlayerController::CallStopJump);
		EnhancedInput->BindAction(InputAction_Jump, ETriggerEvent::Completed, this, &ACombatPlayerController::CallStopJump); 

		// CAMERA CONTROLS
		EnhancedInput->BindAction(InputAction_Look, ETriggerEvent::Triggered, this, &ACombatPlayerController::Look);

		// COMBAT
		EnhancedInput->BindAction(InputAction_LightAttack, ETriggerEvent::Triggered, this, &ACombatPlayerController::CallLightAttack);
		EnhancedInput->BindAction(InputAction_HeavyAttack, ETriggerEvent::Completed, this, &ACombatPlayerController::CallHeavyAttack);

		// LOCK-ON SYSTEM
		EnhancedInput->BindAction(InputAction_LockOn, ETriggerEvent::Triggered, this, &ACombatPlayerController::CallLockOn);
		EnhancedInput->BindAction(InputAction_SwitchEnemyUp, ETriggerEvent::Triggered, this, &ACombatPlayerController::CallSwitchUp);
		EnhancedInput->BindAction(InputAction_SwitchEnemyDown, ETriggerEvent::Triggered, this, &ACombatPlayerController::CallSwitchDown);
	}
}

// Find forward and right vectors based on control rotation and move in those directions
void ACombatPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D MovementValue = Value.Get<FVector2D>();
	FVector ForwardDirection = UKismetMathLibrary::GetForwardVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));
	FVector RightDirection = UKismetMathLibrary::GetRightVector(FRotator(0.0f, GetControlRotation().Yaw, GetControlRotation().Roll));

	if (Player->GetCombatState() == ECombatState::COMBAT_Neutral)
	{
		Player->AddMovementInput(ForwardDirection, MovementValue.Y);
		Player->AddMovementInput(RightDirection, MovementValue.X); 
	}
}

// Call jump functions from player class
void ACombatPlayerController::CallJump()
{
	if (Player->GetCombatState() == ECombatState::COMBAT_Neutral)
		Player->Jump(); 
}

void ACombatPlayerController::CallStopJump()
{
	if (Player)
		Player->StopJumping();
}

// Rotate camera based on input directions 
void ACombatPlayerController::Look(const FInputActionValue& Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();

	AddPitchInput(LookValue.Y);
	AddYawInput(LookValue.X);
}

// Call light attack from player class 
void ACombatPlayerController::CallLightAttack()
{
	if (Player->GetCombatState() == ECombatState::COMBAT_Neutral)
		Player->LightAttackPressed();
}

// Call heavy attack from player class
void ACombatPlayerController::CallHeavyAttack()
{
	if (Player->GetCombatState() == ECombatState::COMBAT_Neutral)
		Player->HeavyAttackPressed(); 
}

// Call lock on from player class
void ACombatPlayerController::CallLockOn()
{
	if (Player)
		Player->LockOn();
}

// Call switch enemy functions from player class
void ACombatPlayerController::CallSwitchUp()
{
	if (Player->bIsLockedOn)
		Player->SwitchEnemyLeft();
}

void ACombatPlayerController::CallSwitchDown()
{
	if (Player->bIsLockedOn)
		Player->SwitchEnemyRight();
}



