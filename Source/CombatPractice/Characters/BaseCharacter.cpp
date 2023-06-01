
#include "BaseCharacter.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"
#include "CombatPractice/Actors/BaseWeapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set default values 
	bUseControllerRotationYaw = false; 
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bCanAttack = true;

	// Create spring arm component and set default values
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	// Create camera component and set default values
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Create a weapon and assign it to the player
	if (WeaponClass)
	{
		Weapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass);
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		Weapon->SetOwner(this);
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ABaseCharacter::MoveRight);

	// Camera control (mouse)
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ABaseCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &ABaseCharacter::AddControllerYawInput);

	// Combat
	PlayerInputComponent->BindAction(TEXT("LightAttack"), EInputEvent::IE_Pressed, this, &ABaseCharacter::LightAttack);
}

// Find forward vector based on control rotation and move player either forwards or backwards
void ABaseCharacter::MoveForward(float Scale)
{
	FVector ForwardDirection = UKismetMathLibrary::GetForwardVector(FRotator(0.0f, Controller->GetControlRotation().Yaw, 0.0f));
	AddMovementInput(ForwardDirection, Scale);
}

// Find right vector based on control rotation and move player either right or left 
void ABaseCharacter::MoveRight(float Scale)
{
	FVector RightDirection = UKismetMathLibrary::GetRightVector(FRotator(0.0f, Controller->GetControlRotation().Yaw, Controller->GetControlRotation().Roll));
	AddMovementInput(RightDirection, Scale);
}

// Perform a light attack
void ABaseCharacter::LightAttack()
{
	if (LightAttackAnimation && bCanAttack)
	{
		PlayAnimMontage(LightAttackAnimation, 1.0f, TEXT("None"));
	}
}

// Apply effects and set variables based on start and end of attack
void ABaseCharacter::AttackEffects(bool bIsStart)
{
	if (bIsStart)
	{
		GetCharacterMovement()->SetJumpAllowed(false);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		bCanAttack = false;
	}
	else
	{
		GetCharacterMovement()->SetJumpAllowed(true);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		bCanAttack = true;
	}
}



