
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Namazu/Actors/Pickup.h"
#include "Namazu/Actors/Weapon.h"
#include "Namazu/Characters/PlayerCharacter.h"
#include "Namazu/Controllers/EnemyAIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyCharacter"), true);
	EnemyController = nullptr; 
	SearchRadius = 500.0f;
	MaxSightAngle = 90.0f; 
	PlayerReference = nullptr;
	AttackRadius = 100.0f;

	// Create health bar 
	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar"));
	HealthBar->SetupAttachment(RootComponent);

	// Create lock-on target
	LockOnIcon = CreateDefaultSubobject<UWidgetComponent>(TEXT("Lock-On Target"));
	LockOnIcon->SetupAttachment(RootComponent);
	LockOnIcon->SetVisibility(false);

	// Create widget for grapple icon
	GrappleIcon = CreateDefaultSubobject<UWidgetComponent>(TEXT("Grapple Icon"));
	GrappleIcon->SetupAttachment(RootComponent);
	GrappleIcon->SetVisibility(false);
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Get a reference to the player
	PlayerReference = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerReference == nullptr)
		return;

	// Get a reference to this enemy's controller
	EnemyController = Cast<AEnemyAIController>(GetController());
	if (EnemyController == nullptr)
		return; 
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 
}

// Getter function to access reference to player
APlayerCharacter* AEnemyCharacter::GetPlayerReference()
{
	return PlayerReference;
}

// Getter function to access lock-on target widget
UWidgetComponent* AEnemyCharacter::GetLockOnTarget()
{
	return LockOnIcon;
}

// Getter function to access this enemy's attack radius 
float AEnemyCharacter::GetAttackRadius()
{
	return AttackRadius;
}

// Set this character to be able to move or not 
void AEnemyCharacter::UpdateMovement(bool bPauseMovement)
{
	Super::UpdateMovement(bPauseMovement); 

	if (bPauseMovement)
		EnemyController->GetPathFollowingComponent()->SetActive(false, true);
	else
		EnemyController->GetPathFollowingComponent()->SetActive(true, true); 
}

// Used to reset variables a character uses during combat
void AEnemyCharacter::ResetAttack()
{
	Super::ResetAttack();

	EnemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("CanRunTree"), true);
}

// Deduct damage from health and update gameplay as needed
void AEnemyCharacter::TakeDamage(FAttackAnimation AttackAnimation, FVector AttackLocation)
{
	Super::TakeDamage(AttackAnimation, AttackLocation); 

	EnemyController->GetBlackboardComponent()->ClearValue(TEXT("CanRunTree"));
}

// Apply effects after the character has finished their death animation 
void AEnemyCharacter::AfterDeath()
{
	Super::AfterDeath();

	SpawnPickup();

	Weapon->Destroy();
	this->Destroy();
}

// Create struct variable from this actor's information
FGrappleActor AEnemyCharacter::CreateGrappleActor()
{
	return FGrappleActor(this, GrappleIcon);
}

// Called when character runs out of health
void AEnemyCharacter::OnDeath()
{
	Super::OnDeath();

	LockOnIcon->SetVisibility(false);
	HealthBar->SetVisibility(false);
	PlayerReference->EnemyDefeated(this); 
}

// Called when character enters fall to death volume
void AEnemyCharacter::OnFallDeath()
{
	Super::OnFallDeath();

	PlayerReference->EnemyDefeated(this);
	Weapon->Destroy();
	this->Destroy();
}

// Calcualte if the player is within the enemy's search radius
bool AEnemyCharacter::IsPlayerClose()
{
	if (PlayerReference)
		return FVector::Dist(GetActorLocation(), PlayerReference->GetActorLocation()) <= SearchRadius;
	else
		return false;
}

// Calculate if the player is within the designted angle
bool AEnemyCharacter::IsPlayerWithinView(bool bCheckBehind, float Angle)
{
	if (PlayerReference)
	{
		// Calculate the angle between the enemy's forward vector and direction to player, and see if player
		FVector DirectionToPlayer = PlayerReference->GetActorLocation() - GetActorLocation();
		DirectionToPlayer.Normalize();

		float AngleBetweenPlayer = UKismetMathLibrary::DegAcos(FVector::DotProduct(GetActorForwardVector(), DirectionToPlayer));

		if (bCheckBehind && AngleBetweenPlayer >= Angle)
			return true;
		else if (AngleBetweenPlayer <= Angle)
			return true;
		else
			return false; 
	}
	else
		return false;
}

// Perform a line trace to see if ray to player is blockd by anything
bool AEnemyCharacter::IsPlayerBlocked()
{
	TArray<AActor*> ActorsToIngore;
	ActorsToIngore.Add(this);

	FHitResult LineResult; 

	return UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), PlayerReference->GetActorLocation(), UEngineTypes::ConvertToTraceType(ECC_Visibility),
												 true, ActorsToIngore, EDrawDebugTrace::None, OUT LineResult, true);
}

// Calulate if the player is within attacking distance
bool AEnemyCharacter::IsPlayerWithinAttackRadius()
{
	return FVector::Dist(GetActorLocation(), PlayerReference->GetActorLocation()) <= AttackRadius;
}

// Spawn a pickup item with random chance
void AEnemyCharacter::SpawnPickup()
{
	int32 Index = FMath::RandRange(0, PickupClasses.Num());

	if (Index != PickupClasses.Num())
		GetWorld()->SpawnActor<APickup>(PickupClasses[Index],
										FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z - GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()),
										FRotator(0.0f));
}
