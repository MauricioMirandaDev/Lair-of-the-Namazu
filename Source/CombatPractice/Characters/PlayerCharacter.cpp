
#include "PlayerCharacter.h"
#include "Animation/AnimMontage.h"
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
	LightAttackAnim_Phase01 = nullptr; 
	LightAttackAnim_Phase02 = nullptr;
	LightAttackAnim_Phase03 = nullptr; 
	LightAttackAnim_Air = nullptr;  
	HeavyAttackAnim = nullptr; 
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

void APlayerCharacter::Jump()
{
	Super::Jump(); 

	bJumpPressed = true;
}

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
void APlayerCharacter::LightAttack()
{
	if (LightAttackAnim_Phase01 && LightAttackAnim_Phase02 && LightAttackAnim_Phase03 && LightAttackAnim_Air && bCanAttack)
	{

		if (GetCharacterMovement()->IsFalling())
			PlayAnimMontage(LightAttackAnim_Air, 1.0f, TEXT("None"));
		else
		{
			switch (AttackCount)
			{
			case 1:
				PlayAnimMontage(LightAttackAnim_Phase01, 1.0f, TEXT("None"));
				break;
			case 2:
				PlayAnimMontage(LightAttackAnim_Phase02, 1.0f, TEXT("None"));
				break;
			case 3:
				PlayAnimMontage(LightAttackAnim_Phase03, 1.0f, TEXT("None"));
				break;
			default:
				break;
			}
		}
	}
}

// Perform animation when player presses heavy attack input
void APlayerCharacter::HeavyAttack()
{
	if (HeavyAttackAnim && bCanAttack)
		PlayAnimMontage(HeavyAttackAnim, 1.0f, TEXT("None")); 
}




