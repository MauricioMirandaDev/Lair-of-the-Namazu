
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

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerCharacter"), true);
	bJumpPressed = false; 
	LockOnCameraOffset = 500.0f;
	MaxLockOnDistance = 100.0f;
	LockedOnEnemy = nullptr; 
	bIsLockedOn = false;
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

	LockOnBehavior();
}

TArray<AEnemyCharacter*> APlayerCharacter::GetNearbyEnemies()
{
	return NearbyEnemies; 
}

// Called when character runs out of health
void APlayerCharacter::OnDeath()
{
	Super::OnDeath(); 

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	SpringArm->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void APlayerCharacter::TakeDamage(FAttackAnimation AttackAnimation, FVector AttackLocation)
{
	Super::TakeDamage(AttackAnimation, AttackLocation); 
}

void APlayerCharacter::SetMovement(bool bPauseMovement)
{
	Super::SetMovement(bPauseMovement);

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

void APlayerCharacter::AfterDeath()
{
	Super::AfterDeath();

	if (ACombatPracticeGameModeBase* GameMode = Cast<ACombatPracticeGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
		GameMode->GameOver();
}

// Perform animation when player inputs light attack
void APlayerCharacter::LightAttackPressed()
{
	if (CombatState == ECombatState::COMBAT_Neutral)
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
}

// Perform animation when player inputs heavy attack
void APlayerCharacter::HeavyAttackPressed()
{
	if (CombatState == ECombatState::COMBAT_Neutral && !GetCharacterMovement()->IsFalling())
		PlayAttackAnim(HeavyAttack);
}

// Trace from the viewport to find all nearby enemies
void APlayerCharacter::TraceForEnemies()
{
	// Variables for trace
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	
	TArray<AActor*> OutActors;

	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(OUT ViewportSize);

	// Get all overlapped enemies within distance
	UKismetSystemLibrary::BoxOverlapActors(GetWorld(), GetActorLocation() + (Camera->GetForwardVector() * MaxLockOnDistance),
										   FVector(MaxLockOnDistance, ViewportSize.X, ViewportSize.Y), ObjectTypes, AEnemyCharacter::StaticClass(), ActorsToIgnore,
										   OutActors);

	for (AActor* Actor : OutActors)
	{
		if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Actor))
			NearbyEnemies.Add(Enemy);
	}
}

// Determine the closest enemy to the player
AEnemyCharacter* APlayerCharacter::DetermineClosestEnemy()
{
	AEnemyCharacter* ClosestEnemy = NearbyEnemies[0];

	for (AEnemyCharacter* Enemy : NearbyEnemies)
	{
		if (FVector::Dist(GetActorLocation(), Enemy->GetActorLocation()) < FVector::Dist(GetActorLocation(), ClosestEnemy->GetActorLocation()))
			ClosestEnemy = Enemy;
	}

	return ClosestEnemy;
}

// Lock onto a nearby enemy or stop locking on; enables Tick() when the player locks onto an enemy and disables Tick() when the player stops
void APlayerCharacter::LockOntoEnemy()
{
	TraceForEnemies();

	if (!bIsLockedOn && NearbyEnemies.Num() > 0)
	{
		bIsLockedOn = true;
		LockedOnEnemy = DetermineClosestEnemy();
		LockedOnEnemy->GetLockOnTarget()->SetVisibility(true);
		SetActorTickEnabled(true);
	}
	else
	{
		bIsLockedOn = false;
		LockedOnEnemy->GetLockOnTarget()->SetVisibility(false);
		LockedOnEnemy = nullptr;
		SetActorTickEnabled(false);
	}
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

// Moves player according to locked on enemy and continues to trace for nearby enemies; disables Tick() when there are no nearby enemies
void APlayerCharacter::LockOnBehavior()
{
	if (NearbyEnemies.Num() == 0)
	{
		bIsLockedOn = false;
		LockedOnEnemy->GetLockOnTarget()->SetVisibility(false);
		LockedOnEnemy = nullptr;
		SetActorTickEnabled(false);
	}
	else
	{
		// If the current locked on enemy dies, lock onto the nearest one next
		if (!NearbyEnemies.Contains(LockedOnEnemy))
		{
			LockedOnEnemy = DetermineClosestEnemy();
			LockedOnEnemy->GetLockOnTarget()->SetVisibility(true);
		}

		LockedOnMovement();
		TraceForEnemies();
	}
}

// Functions to switch between nearby enemies in both directions
void APlayerCharacter::SwitchEnemyUp()
{
	LockedOnEnemy->GetLockOnTarget()->SetVisibility(false);

	int32 CurrentIndex = NearbyEnemies.Find(LockedOnEnemy);
	CurrentIndex++;

	if (CurrentIndex >= NearbyEnemies.Num())
		LockedOnEnemy = NearbyEnemies[0];
	else
		LockedOnEnemy = NearbyEnemies[CurrentIndex];

	LockedOnEnemy->GetLockOnTarget()->SetVisibility(true);
}

void APlayerCharacter::SwitchEnemyDown()
{
	LockedOnEnemy->GetLockOnTarget()->SetVisibility(false);

	int32 CurrentIndex = NearbyEnemies.Find(LockedOnEnemy);
	CurrentIndex--;

	if (CurrentIndex < 0)
		LockedOnEnemy = NearbyEnemies[NearbyEnemies.Num() - 1];
	else
		LockedOnEnemy = NearbyEnemies[CurrentIndex];

	LockedOnEnemy->GetLockOnTarget()->SetVisibility(true);
}





