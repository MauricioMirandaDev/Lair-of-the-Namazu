
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
void APlayerCharacter::RemoveEnemyFromNearbyEnemies(AActor* Enemy)
{
	if (NearbyEnemies.Contains(Enemy))
		NearbyEnemies.Remove(Enemy); 

	if (LockedOnEnemy == Enemy)
		LockedOnEnemy = nullptr; 
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

// Lock onto a nearby enemy or stop locking on; enables Tick() when the player locks onto an enemy and disables Tick() when the player stops
void APlayerCharacter::LockOntoEnemy()
{
	TraceForEnemies();

	if (!bIsLockedOn && NearbyEnemies.Num() > 0)
		BeginLockingOn(); 
	else
		StopLockingOn(); 
}

// Trace from the viewport to find all nearby enemies
void APlayerCharacter::TraceForEnemies()
{
	// Variables for traces 
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));

	TArray<AActor*> OutActors; 

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(OUT ViewportSize);

	FVector CameraForward = UKismetMathLibrary::GetForwardVector(Camera->GetComponentRotation());

	FHitResult HitResult; 

	// Get all overlapped enemies within distance
	UKismetSystemLibrary::BoxOverlapActors(GetWorld(), GetActorLocation() + (CameraForward * MaxLockOnDistance), FVector(MaxLockOnDistance, ViewportSize.X, ViewportSize.Y), 
										   ObjectTypes, AEnemyCharacter::StaticClass(), ActorsToIgnore, OutActors);

	// Save all detected enemies; remove the enemy if line of sight to it is blocked
	for (AActor* Enemy : OutActors)
	{
		if (!NearbyEnemies.Contains(Enemy))
			NearbyEnemies.Add(Enemy);

		UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), Enemy->GetActorLocation(), UEngineTypes::ConvertToTraceType(ECC_Visibility), true, ActorsToIgnore,
											  EDrawDebugTrace::None, OUT HitResult, true); 

		if (HitResult.bBlockingHit)
			NearbyEnemies.Remove(Enemy);
	}
}

// Determine the closest enemy to the player
AEnemyCharacter* APlayerCharacter::DetermineClosestEnemy()
{
	AActor* ClosestEnemy = NearbyEnemies[0];

	for (AActor* Enemy : NearbyEnemies)
	{
		if (FVector::Dist(GetActorLocation(), Enemy->GetActorLocation()) < FVector::Dist(GetActorLocation(), ClosestEnemy->GetActorLocation()))
			ClosestEnemy = Enemy;
	}

	return Cast<AEnemyCharacter>(ClosestEnemy);
}

// Moves player according to locked on enemy and continues to trace for nearby enemies; disables Tick() when there are no nearby enemies
void APlayerCharacter::LockOnBehavior()
{
	if (NearbyEnemies.IsEmpty())
		StopLockingOn();
	else
	{
		// If the current locked on enemy is lost, lock onto the closest enemy next 
		if (LockedOnEnemy == nullptr)
		{
			LockedOnEnemy = DetermineClosestEnemy();
			LockedOnEnemy->GetLockOnTarget()->SetVisibility(true);
		}

		LockedOnMovement();
		TraceForEnemies();
	}
}

// Begin locking onto an enemy
void APlayerCharacter::BeginLockingOn()
{
	bIsLockedOn = true;
	LockedOnEnemy = DetermineClosestEnemy();
	LockedOnEnemy->GetLockOnTarget()->SetVisibility(true);
	SetActorTickEnabled(true);
}

// Stops locking onto an enemy
void APlayerCharacter::StopLockingOn()
{
	bIsLockedOn = false;
	if (LockedOnEnemy)
		LockedOnEnemy->GetLockOnTarget()->SetVisibility(false);
	LockedOnEnemy = nullptr;
	NearbyEnemies.Empty();
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

// Functions to switch between nearby enemies in both directions
void APlayerCharacter::SwitchEnemyUp()
{
	LockedOnEnemy->GetLockOnTarget()->SetVisibility(false);

	int32 CurrentIndex = NearbyEnemies.Find(LockedOnEnemy);
	CurrentIndex++;

	if (CurrentIndex >= NearbyEnemies.Num())
		LockedOnEnemy = Cast<AEnemyCharacter>(NearbyEnemies[0]);
	else
		LockedOnEnemy = Cast<AEnemyCharacter>(NearbyEnemies[CurrentIndex]);

	LockedOnEnemy->GetLockOnTarget()->SetVisibility(true);
}

void APlayerCharacter::SwitchEnemyDown()
{
	LockedOnEnemy->GetLockOnTarget()->SetVisibility(false);

	int32 CurrentIndex = NearbyEnemies.Find(LockedOnEnemy);
	CurrentIndex--;

	if (CurrentIndex < 0)
		LockedOnEnemy = Cast<AEnemyCharacter>(NearbyEnemies[NearbyEnemies.Num() - 1]);
	else
		LockedOnEnemy = Cast<AEnemyCharacter>(NearbyEnemies[CurrentIndex]);

	LockedOnEnemy->GetLockOnTarget()->SetVisibility(true);
}





