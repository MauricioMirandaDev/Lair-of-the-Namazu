
#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "CombatPractice/CombatPlayerController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerCharacter"), true);
	bJumpPressed = false; 
	bCanAttack = true;
	AttackCount = 0;

	// Create spring arm component and set default values
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	// Create camera component and set default values
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	ControllerRef = Cast<ACombatPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

// Called whenever the player performs a jump 
void APlayerCharacter::Jump()
{
	Super::Jump(); 

	bJumpPressed = true;
}

// Called when the player's movement mode changes
void APlayerCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode); 

	if (PrevMovementMode == EMovementMode::MOVE_Falling)
		bJumpPressed = false;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when character runs out of health
void APlayerCharacter::OnDeath()
{
	Super::OnDeath(); 

	if (ControllerRef)
		ControllerRef->DisableInput(ControllerRef);
}

// Perform animation when player presses light attack input 
void APlayerCharacter::LightAttackPressed()
{
	if (bCanAttack)
	{
		if (GetCharacterMovement()->IsFalling())
		{
			PlayAnimMontage(LightAttack_Air.Animation, 1.0f, TEXT("None"));
			CurrentAttackAnimation = LightAttack_Air;
		}
		else
		{
			AttackCount++;

			switch (AttackCount)
			{
			case 1:
				PlayAnimMontage(LightAttack_Phase01.Animation, 1.0f, TEXT("None"));
				CurrentAttackAnimation = LightAttack_Phase01;
				break;
			case 2:
				PlayAnimMontage(LightAttack_Phase02.Animation, 1.0f, TEXT("None"));
				CurrentAttackAnimation = LightAttack_Phase02;
				break;
			case 3:
				PlayAnimMontage(LightAttack_Phase03.Animation, 1.0f, TEXT("None"));
				CurrentAttackAnimation = LightAttack_Phase03;
				break;
			default:
				break;
			}
		}
	}
}

// Perform animation when player presses heavy attack input
void APlayerCharacter::HeavyAttackPressed()
{
	if (bCanAttack && !GetCharacterMovement()->IsFalling())
	{
		PlayAnimMontage(HeavyAttack.Animation, 1.0f, TEXT("None"));
		CurrentAttackAnimation = HeavyAttack;
	}
}




