
#include "PlayerCharacter.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerCharacter"), true);
	LightAttackAnimation = nullptr; 
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
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacter::MoveRight); 

	// Camera control
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APlayerCharacter::AddControllerYawInput);

	// Combat
	PlayerInputComponent->BindAction(TEXT("LightAttack"), EInputEvent::IE_Pressed, this, &APlayerCharacter::LightAttack);
}

// Called when character runs out of health
void APlayerCharacter::OnDeath()
{
	Super::OnDeath(); 
}

// Find forward vector based on control rotation and move player either forwards or backwards
void APlayerCharacter::MoveForward(float Scale)
{
	FVector ForwardDirection = UKismetMathLibrary::GetForwardVector(FRotator(0.0f, Controller->GetControlRotation().Yaw, 0.0f));
	AddMovementInput(ForwardDirection, Scale);
}

// Find right vector based on control rotation and move player either right or left 
void APlayerCharacter::MoveRight(float Scale)
{
	FVector RightDirection = UKismetMathLibrary::GetRightVector(FRotator(0.0f, Controller->GetControlRotation().Yaw, Controller->GetControlRotation().Roll));
	AddMovementInput(RightDirection, Scale);
}

// Perform animation when player presses light attack button 
void APlayerCharacter::LightAttack()
{
	if (LightAttackAnimation && bCanAttack)
		PlayAnimMontage(LightAttackAnimation, 1.0f, TEXT("None"));
}




