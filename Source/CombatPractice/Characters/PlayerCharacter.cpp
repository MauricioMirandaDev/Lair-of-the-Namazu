
#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "CombatPractice/CombatPracticeGameModeBase.h"
#include "CombatPractice/Actors/GrappleActor.h"
#include "CombatPractice/Actors/Rope.h"
#include "CombatPractice/Characters/EnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

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
	CastRopeAnim = nullptr; 
	TensionStrength = 100.0f;
	InitialPosition = FVector(0.0f);
	EndPosition = FVector(0.0f);
	bCanGrapple = false; 
	bIsGrappling = false; 

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

	DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed; 

	// Create rope actor and attach to player
	if (RopeClass)
	{
		Rope = GetWorld()->SpawnActor<ARope>(RopeClass);
		Rope->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("RopeSocket"));
		Rope->SetPlayerReference(this); 
	}

	SetActorTickEnabled(false);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LockOnBehavior();
}

// Set whether the player can grapple or not
void APlayerCharacter::SetCanGrapple(bool bCanPlayerGrapple)
{
	bCanGrapple = bCanPlayerGrapple;
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

	//StopLockingOn(); 
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

	TArray<AActor*> FoundActors;
	TArray<AEnemyCharacter*> NearbyEnemies;

	FHitResult HitResult;

	// Get all available enemies within lock on radius
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), LockedOnEnemy->GetActorLocation(), MaxLockOnDistance, ObjectTypes, AEnemyCharacter::StaticClass(), ActorsToIgnore, FoundActors);

	// Check to see which enemies are blocked by an object
	for (AActor* Actor : FoundActors)
	{
		if (!UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), Actor->GetActorLocation(), UEngineTypes::ConvertToTraceType(ECC_Visibility), true, ActorsToIgnore,
			EDrawDebugTrace::None, OUT HitResult, true))
			NearbyEnemies.Add(Cast<AEnemyCharacter>(Actor));
	}

	// Lock onto the nearest enemy based on the desired direction
	if (!NearbyEnemies.IsEmpty())
	{
		AEnemyCharacter* ClosestNearbyEnemy = NearbyEnemies[0];

		for (AEnemyCharacter* Enemy : NearbyEnemies)
		{
			FVector DirectionToNearbyEnemy = Enemy->GetActorLocation() - LockedOnEnemy->GetActorLocation();
			DirectionToNearbyEnemy.Normalize();

			FVector DirectionToClosest = ClosestNearbyEnemy->GetActorLocation() - LockedOnEnemy->GetActorLocation();
			DirectionToClosest.Normalize();

			if (FVector::DotProduct(Direction, DirectionToNearbyEnemy) > FVector::DotProduct(Direction, DirectionToClosest))
				ClosestNearbyEnemy = Enemy;
		}

		return ClosestNearbyEnemy;
	}
	else
		return nullptr;
}

// Switch which enemy the player is locking onto based on the desired direction 
void APlayerCharacter::SwitchLockedOnEnemy(FVector Direction)
{
	if (LockedOnEnemy && FindNearbyEnemy(Direction))
	{
		LockedOnEnemy->GetLockOnTarget()->SetVisibility(false);

		LockedOnEnemy = FindNearbyEnemy(Direction);
		LockedOnEnemy->GetLockOnTarget()->SetVisibility(true);
	}
}

// Prepare variables for grappling 
void APlayerCharacter::PrepareGrapple()
{
	InitialPosition = GetActorLocation();
	EndPosition = Rope->GetTarget().Actor->GetActorLocation();

	FVector DirectionToTarget = EndPosition - InitialPosition;
	DirectionToTarget.Normalize();

	FRotator RotationToPoint = DirectionToTarget.Rotation();
	SetActorRotation(FRotator(0.0f, RotationToPoint.Yaw, 0.0f));
}

// Reset values after grappling
void APlayerCharacter::FinishGrapple()
{
	Rope->SetActorTickEnabled(true);
	Rope->UpdateRopeAttached(false);
	bIsGrappling = false;
}

// Use linear interpolation to grapple the player to a point
void APlayerCharacter::LerpPlayerPosition(float Alpha)
{
	FVector LerpedLocation = FMath::Lerp(InitialPosition, EndPosition, Alpha);

	SetActorLocation(LerpedLocation);
}

// Play animation to cast or detach rope
void APlayerCharacter::CastRope()
{
	if (!bIsGrappling && bCanGrapple)
	{
		FVector DirectionToTarget = Rope->GetTarget().Actor->GetActorLocation() - GetActorLocation();
		DirectionToTarget.Normalize();

		FRotator RotationToTarget = DirectionToTarget.Rotation();

		SetActorRotation(FRotator(0.0f, RotationToTarget.Yaw, 0.0f));

		PlayAnimMontage(CastRopeAnim, 1.0f, TEXT("None"));
		Rope->SetActorTickEnabled(false);
		bIsGrappling = true;
	}
	else if (bIsGrappling)
	{
		Rope->SetActorTickEnabled(true);
		Rope->UpdateRopeAttached(false);
		bIsGrappling = false;
	}
}

// Add tension to player while in midair
void APlayerCharacter::AddTensionForce()
{
	FVector DirectionToPlayer = GetActorLocation() - Rope->GetTarget().Actor->GetActorLocation();
	DirectionToPlayer.Normalize();

	FVector Tension = DirectionToPlayer * FVector::DotProduct(GetVelocity(), DirectionToPlayer);

	GetCharacterMovement()->AddForce(Tension * -TensionStrength * GetCharacterMovement()->Mass);
}

// Attach rope during animation
void APlayerCharacter::AttachRope()
{
	Rope->UpdateRopeAttached(true);
	Rope->GetTarget().SetIconVisibility(false);
}