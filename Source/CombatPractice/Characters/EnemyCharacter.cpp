
#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
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

// Called when character runs out of health
void AEnemyCharacter::OnDeath()
{
	Super::OnDeath();

	GetController()->UnPossess();
	LockOnTarget->SetVisibility(false);
	HealthBar->DestroyComponent();

	if (PlayerReference->GetNearbyEnemies().Contains(this))
		PlayerReference->GetNearbyEnemies().Remove(this);
}

void AEnemyCharacter::ResetAttack()
{
	Super::ResetAttack();

	if (AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController()))
		AIController->GetBlackboardComponent()->ClearValue(TEXT("HitSuccessful"));
}

void AEnemyCharacter::AfterDeath()
{
	Super::AfterDeath();

	Weapon->Destroy();
	this->Destroy();
}

// Perform a line of sight calculation to determine if the enemy can see the player
bool AEnemyCharacter::CanSeePlayer()
{
	if (IsPlayerClose())
	{
		// Calculate the angle between the enemy's forward vector and direction to player
		FVector DirectionToPlayer = PlayerReference->GetActorLocation() - GetActorLocation();
		DirectionToPlayer.Normalize();

		float AngleBetweenPlayer = UKismetMathLibrary::DegAcos(FVector::DotProduct(GetActorForwardVector(), DirectionToPlayer));

		if (AngleBetweenPlayer <= MaxSightAngle && !IsPlayerBlocked())
			return true;
	}

	return false;
}

// Perform a sphere trace to see if the player is withing detection radius
bool AEnemyCharacter::IsPlayerClose()
{
	if (PlayerReference != nullptr)
		return FVector::Dist(GetActorLocation(), PlayerReference->GetActorLocation()) <= SearchRadius;
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
bool AEnemyCharacter::IsReadyToAttack()
{
	return FVector::Dist(GetActorLocation(), PlayerReference->GetActorLocation()) <= AttackRadius && CombatState == ECombatState::COMBAT_Neutral;
}
