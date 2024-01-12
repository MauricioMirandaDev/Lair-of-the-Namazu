
#include "PlayerCharacter.h"
#include "CableComponent.h"
#include "Camera/CameraComponent.h"
#include "CombatPractice/CombatPracticeGameModeBase.h"
#include "CombatPractice/Characters/EnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerCharacter"), true);
	bJumpPressed = false; 
	LockOnCameraOffset = 500.0f;
	MaxLockOnDistance = 100.0f;
	bIsLockedOn = false;
	LockedOnEnemy = nullptr; 
	bEnemyJustDefeated = false; 
	RopeLength = 500.0f;

	// Create spring arm component and set default values
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	// Create camera component and set default values
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// Create a scene component where the rope will be held
	RopeHolster = CreateDefaultSubobject<USceneComponent>(TEXT("Rope Holster"));
	RopeHolster->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("RopeSocket"));

	// Create a cable component and set default values
	Rope = CreateDefaultSubobject<UCableComponent>(TEXT("Rope"));
	Rope->AttachToComponent(RopeHolster, FAttachmentTransformRules::KeepRelativeTransform);
	Rope->bAttachStart = true;
	Rope->bAttachEnd = true;
	Rope->CableLength = RopeLength;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);
	DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed; 
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LockOnBehavior();
}

// Set this character to be able to move or not 
void APlayerCharacter::UpdateMovement(bool bPauseMovement)
{
	Super::UpdateMovement(bPauseMovement);

	if (bPauseMovement)
		GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	else
		GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed; 
}

// Used to reset variables a character uses during combat
void APlayerCharacter::ResetAttack()
{
	Super::ResetAttack();
}

// Deduct damage from health and update gameplay as needed
void APlayerCharacter::TakeDamage(FAttackAnimation AttackAnimation, FVector AttackLocation)
{
	Super::TakeDamage(AttackAnimation, AttackLocation); 

	StopLockingOn(); 
}

// Apply effects after the character has finished their death animation 
void APlayerCharacter::AfterDeath()
{
	Super::AfterDeath();

	if (ACombatPracticeGameModeBase* GameMode = Cast<ACombatPracticeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
		GameMode->GameOver();
}

// Remove the designated enemy from NearbyEnemies if it is present in the array; stop locking onto the enemy if it was previously 
void APlayerCharacter::EnemyDefeated(AActor* Enemy)
{
	if (LockedOnEnemy == Enemy)
	{
		LockedOnEnemy = nullptr; 
		bEnemyJustDefeated = true; 
	}
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

// Called when character runs out of health
void APlayerCharacter::OnDeath()
{
	Super::OnDeath(); 

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	SpringArm->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

// Perform animation when player inputs light attack
void APlayerCharacter::LightAttackPressed()
{
	if (GetCharacterMovement()->IsFalling())
	{
		PlayAttackAnim(JumpAttack);
	}
	else
	{
		int32 SelectedVariation = FMath::RandRange(0, LightAttackVariations.Num() - 1);

		switch (SelectedVariation)
		{
		case 0:
			LightAttack.Animation = LightAttackVariations[0];
			break;
		case 1:
			LightAttack.Animation = LightAttackVariations[1];
			break;
		default:
			LightAttack.Animation = LightAttackVariations[LightAttackVariations.Num() - 1];
			break;
		}

		PlayAttackAnim(LightAttack);
	}
}

// Perform animation when player inputs heavy attack
void APlayerCharacter::HeavyAttackPressed()
{
	if (!GetCharacterMovement()->IsFalling())
		PlayAttackAnim(HeavyAttack);
}

// Lock onto a nearby enemy or stop locking on
void APlayerCharacter::LockOn()
{
	if (!bIsLockedOn)
		BeginLockingOn();
	else
		StopLockingOn();
}

// Trace to find an enemy that can be locked onto
AEnemyCharacter* APlayerCharacter::FindClosestEnemy()
{
	// Variables for traces 
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	TArray<AActor*> FoundActors;
	TArray<AEnemyCharacter*> NearbyEnemies;

	FVector CameraForward = UKismetMathLibrary::GetForwardVector(Camera->GetComponentRotation());

	FHitResult HitResult; 

	// Get all available enemies within lock on radius
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), MaxLockOnDistance, ObjectTypes, AEnemyCharacter::StaticClass(), ActorsToIgnore, FoundActors);
	
	// Check to see which enemies are blocked by an object
	for (AActor* Actor : FoundActors)
	{
		if (!UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), Actor->GetActorLocation(), UEngineTypes::ConvertToTraceType(ECC_Visibility), true, ActorsToIgnore,
												   EDrawDebugTrace::None, OUT HitResult, true))
			NearbyEnemies.Add(Cast<AEnemyCharacter>(Actor));
	}
	
	// Lock onto the enemy closest to the center of the camera
	if (!NearbyEnemies.IsEmpty())
	{
		AEnemyCharacter* ClosestEnemy = NearbyEnemies[0];

		for (AEnemyCharacter* Enemy : NearbyEnemies)
		{
			FVector DirectionToEnemy = Enemy->GetActorLocation() - GetActorLocation();
			DirectionToEnemy.Normalize();

			FVector DirectionToClosest = ClosestEnemy->GetActorLocation() - GetActorLocation();
			DirectionToClosest.Normalize();

			if (FVector::DotProduct(CameraForward, DirectionToEnemy) > FVector::DotProduct(CameraForward, DirectionToClosest))
				ClosestEnemy = Enemy;
		}

		return ClosestEnemy;
	}
	else
		return nullptr;
}

// Moves player based on the enemy they are locking onto
void APlayerCharacter::LockOnBehavior()
{
	// If the player defeats the locked on enemy, find the next nearest enemy to lock onto or stop if there are none 
	if (LockedOnEnemy == nullptr)
	{
		if (bEnemyJustDefeated)
		{
			BeginLockingOn();
			bEnemyJustDefeated = false;
		}
		else
			StopLockingOn();
	}
	else
		LockedOnMovement();
}

// Begin locking onto an enemy if one can be locked onto; enables Tick() 
void APlayerCharacter::BeginLockingOn()
{
	LockedOnEnemy = FindClosestEnemy();
	if (LockedOnEnemy)
	{
		bIsLockedOn = true;
		LockedOnEnemy->GetLockOnTarget()->SetVisibility(true);
		SetActorTickEnabled(true);
	}
}

// Stops locking onto an enemy; disables Tick()
void APlayerCharacter::StopLockingOn()
{
	bIsLockedOn = false;
	if (LockedOnEnemy)
		LockedOnEnemy->GetLockOnTarget()->SetVisibility(false);
	LockedOnEnemy = nullptr;
	SetActorTickEnabled(false); 
}

// Set the player and camera to always face the locked on enemy
void APlayerCharacter::LockedOnMovement()
{
	// Rotate the player to always face the enemy
	FVector DirectionToEnemy = LockedOnEnemy->GetActorLocation() - GetActorLocation();
	DirectionToEnemy.Normalize();

	SetActorRotation(DirectionToEnemy.Rotation());

	// Rotate the camera to focus on enemy and player from a further vantage point 
	FVector CameraToEnemy = LockedOnEnemy->GetActorLocation() - (GetActorLocation() + (GetActorForwardVector() * -LockOnCameraOffset) + FVector(0.0f, 0.0f, LockOnCameraOffset));
	CameraToEnemy.Normalize();

	GetController()->SetControlRotation(CameraToEnemy.Rotation());
}

// Find a nearby enemy to switch to when locking on
AEnemyCharacter* APlayerCharacter::FindNearbyEnemy(FVector Direction)
{
	// Variables for traces
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(LockedOnEnemy);

	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(OUT ViewportSize);

	FHitResult BoxResult, LineResult;

	// Perform a box trace in the desired direction
	if (UKismetSystemLibrary::BoxTraceSingleForObjects(GetWorld(), LockedOnEnemy->GetActorLocation(), LockedOnEnemy->GetActorLocation() + (Direction * MaxLockOnDistance),
													   FVector(10.0f, ViewportSize.X, ViewportSize.Y), Direction.Rotation(), ObjectTypes, true, ActorsToIgnore,
													   EDrawDebugTrace::None, OUT BoxResult, true))
	{
		// If an enemy is found, check there is nothing blocking line of sight to the enemy
		if (!UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), BoxResult.ImpactPoint, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, ActorsToIgnore,
												   EDrawDebugTrace::None, OUT LineResult, true))
			return Cast<AEnemyCharacter>(BoxResult.GetActor());
		else
			return nullptr;
	}

	return nullptr;
}

// Functions to switch between nearby enemies in both directions
void APlayerCharacter::SwitchEnemyLeft()
{
	FVector Left = UKismetMathLibrary::GetRightVector(Camera->GetComponentRotation()) * -1.0f;

	if (LockedOnEnemy && FindNearbyEnemy(Left))
	{
		LockedOnEnemy->GetLockOnTarget()->SetVisibility(false);

		LockedOnEnemy = FindNearbyEnemy(Left);
		LockedOnEnemy->GetLockOnTarget()->SetVisibility(true);
	}
}

void APlayerCharacter::SwitchEnemyRight()
{
	FVector Right = UKismetMathLibrary::GetRightVector(Camera->GetComponentRotation());

	if (LockedOnEnemy && FindNearbyEnemy(Right))
	{
		LockedOnEnemy->GetLockOnTarget()->SetVisibility(false);

		LockedOnEnemy = FindNearbyEnemy(Right);
		LockedOnEnemy->GetLockOnTarget()->SetVisibility(true);
	}
}





