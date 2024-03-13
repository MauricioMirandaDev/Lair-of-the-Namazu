
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
	InputAction_LookGamepad = nullptr; 
	InputAction_Jump = nullptr; 
	InputAction_LightAttack = nullptr; 
	InputAction_HeavyAttack = nullptr; 
	InputAction_LockOn = nullptr;
	InputAction_SwitchEnemy = nullptr; 
	InputAction_CastRope = nullptr; 

	GamepadLookRate = 100.0f; 
	bCanSwitchEnemy = true;  
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
		EnhancedInput->BindAction(InputAction_LookGamepad, ETriggerEvent::Triggered, this, &ACombatPlayerController::LookGamepad);

		// COMBAT
		EnhancedInput->BindAction(InputAction_LightAttack, ETriggerEvent::Triggered, this, &ACombatPlayerController::CallLightAttack);
		EnhancedInput->BindAction(InputAction_HeavyAttack, ETriggerEvent::Completed, this, &ACombatPlayerController::CallHeavyAttack);

		// LOCK-ON SYSTEM
		//EnhancedInput->BindAction(InputAction_LockOn, ETriggerEvent::Triggered, this, &ACombatPlayerController::CallLockOn);
		//EnhancedInput->BindAction(InputAction_SwitchEnemy, ETriggerEvent::Triggered, this, &ACombatPlayerController::CallSwitchEnemy);
		//EnhancedInput->BindAction(InputAction_SwitchEnemy, ETriggerEvent::Completed, this, &ACombatPlayerController::ResetSwitchEnemy);

		// ROPE
		EnhancedInput->BindAction(InputAction_CastRope, ETriggerEvent::Triggered, this, &ACombatPlayerController::CallCastRope); 
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

// Rotate camera based on input from gamepad
void ACombatPlayerController::LookGamepad(const FInputActionValue& Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();

	AddPitchInput(LookValue.Y * GamepadLookRate * GetWorld()->GetDeltaSeconds());
	AddYawInput(LookValue.X * GamepadLookRate * GetWorld()->GetDeltaSeconds());
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

/*
// Call lock on from player class
void ACombatPlayerController::CallLockOn()
{
	if (Player)
		Player->LockOn();
}

// Call switch enemy from player class 
void ACombatPlayerController::CallSwitchEnemy(const FInputActionValue& Value)
{
	const FVector2D Input = Value.Get<FVector2D>();

	if (Player->bIsLockedOn && bCanSwitchEnemy)
	{
		bCanSwitchEnemy = false;

		FVector LockOnDirection = FVector::ZeroVector;

		// Create a direction based on the player's input and current rotation 
		if (Input.X == 1.0f)
			LockOnDirection += Player->GetActorRightVector();
		else if (Input.X == -1.0f)
			LockOnDirection += -Player->GetActorRightVector();

		if (Input.Y == 1.0f)
			LockOnDirection += Player->GetActorForwardVector();
		else if (Input.Y == -1.0f)
			LockOnDirection += -Player->GetActorForwardVector();

		LockOnDirection.Normalize();

		Player->SwitchLockedOnEnemy(LockOnDirection);
	}
}

// Allow the player to switch after releasing input 
void ACombatPlayerController::ResetSwitchEnemy()
{
	bCanSwitchEnemy = true;
}
*/

// Call rope cast from player class 
void ACombatPlayerController::CallCastRope()
{
	if (Player)
		Player->CastRope(); 
}


