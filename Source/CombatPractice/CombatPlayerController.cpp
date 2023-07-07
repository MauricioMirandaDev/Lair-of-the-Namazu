
#include "CombatPlayerController.h"
#include "CombatPractice/Characters/PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

// Called when the game starts
void ACombatPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeGameOnly());

	Player = Cast<APlayerCharacter>(GetPawn());
}

// Called to bind functionality to input
void ACombatPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Camera control
	InputComponent->BindAxis(TEXT("LookUp"), this, &ACombatPlayerController::AddPitchInput);
	InputComponent->BindAxis(TEXT("LookRight"), this, &ACombatPlayerController::AddYawInput);

	// Movement
	InputComponent->BindAxis(TEXT("MoveForward"), this, &ACombatPlayerController::MoveForward);
	InputComponent->BindAxis(TEXT("MoveRight"), this, &ACombatPlayerController::MoveRight);

	// Combat
	InputComponent->BindAction(TEXT("LightAttack"), IE_Pressed, this, &ACombatPlayerController::CallLightAttack);
}

// Find forward vector based on control rotation and move player either forwards or backwards
void ACombatPlayerController::MoveForward(float Scale)
{
	FVector ForwardDirection = UKismetMathLibrary::GetForwardVector(FRotator(0.0f, GetControlRotation().Yaw, 0.0f));

	if (GetPawn())
		GetPawn()->AddMovementInput(ForwardDirection, Scale);
}

// Find right vector based on control rotation and move player either right or left 
void ACombatPlayerController::MoveRight(float Scale)
{
	FVector RightDirection = UKismetMathLibrary::GetRightVector(FRotator(0.0f, GetControlRotation().Yaw, GetControlRotation().Roll));

	if (GetPawn())
		GetPawn()->AddMovementInput(RightDirection, Scale);
}

// Call light attack from player class 
void ACombatPlayerController::CallLightAttack()
{
	if (Player)
		Player->LightAttack();
}

