
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BrainComponent.h" 
#include "CombatPractice/AI/EnemyAIController.h"
#include "CombatPractice/Characters/PlayerCharacter.h"
#include "CombatPractice/Actors/Weapon.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyCharacter"), true);
	SearchRadius = 500.0f;
	MaxSightAngle = 90.0f; 
	AttackRadius = 100.0f;
	EnemyController = nullptr; 
	PlayerReference = nullptr;

	// Create health bar 
	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar"));
	HealthBar->SetupAttachment(RootComponent);

	// Create lock-on target
	LockOnTarget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Lock-On Target"));
	LockOnTarget->SetupAttachment(RootComponent);
	LockOnTarget->SetVisibility(false);
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
	return LockOnTarget;
}

float AEnemyCharacter::GetAttackRadius()
{
	return AttackRadius;
}

// Called when character runs out of health
void AEnemyCharacter::OnDeath()
{
	Super::OnDeath();

	EnemyController->GetBlackboardComponent()->ClearValue(TEXT("CanRunTree"));
	EnemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("IsSelfDead"), true);
	//EnemyController->UnPossess();
	LockOnTarget->SetVisibility(false);
	HealthBar->SetVisibility(false);

	if (PlayerReference->GetNearbyEnemies().Contains(this))
		PlayerReference->GetNearbyEnemies().Remove(this);
}

void AEnemyCharacter::SetMovement(bool bPauseMovement)
{
	Super::SetMovement(bPauseMovement); 

	if (bPauseMovement)
		EnemyController->GetPathFollowingComponent()->SetActive(false, true);
	else
		EnemyController->GetPathFollowingComponent()->SetActive(true, true); 
}

void AEnemyCharacter::ResetAttack()
{
	Super::ResetAttack();

	EnemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("CanRunTree"), true);
}

void AEnemyCharacter::AfterDeath()
{
	Super::AfterDeath();

	Weapon->Destroy();
	this->Destroy();
}

void AEnemyCharacter::TakeDamage(FAttackAnimation AttackAnimation, FVector AttackLocation)
{
	Super::TakeDamage(AttackAnimation, AttackLocation); 

	EnemyController->GetBlackboardComponent()->SetValueAsBool(TEXT("SeePlayer"), true);
	EnemyController->GetBlackboardComponent()->ClearValue(TEXT("CanRunTree"));
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
bool AEnemyCharacter::PlayerWithinAttackRadius()
{
	return FVector::Dist(GetActorLocation(), PlayerReference->GetActorLocation()) <= AttackRadius;
}
