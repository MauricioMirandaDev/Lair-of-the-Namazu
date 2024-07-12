
#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Namazu/Characters/EnemyCharacter.h"
#include "Namazu/Characters/PlayerCharacter.h"

// Set default values
AEnemyAIController::AEnemyAIController()
{
	BehaviorTree = nullptr; 
	ChanceOfAlternateAttack = 0.0f;
	EnemyOwner = nullptr; 
	PatrolSelection = 0;
}

// Called when the game starts or when spawned
void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	EnemyOwner = Cast<AEnemyCharacter>(GetPawn());
	if (EnemyOwner == nullptr)
		return; 
	
	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);

		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
		GetBlackboardComponent()->SetValueAsBool(TEXT("CanRunTree"), true);
	}
}

// Called every frame
void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

// Getter function to access the enemy who owns this controller
AEnemyCharacter* AEnemyAIController::GetEnemyOwner()
{
	return EnemyOwner;
}

// Getter function to access the player's location 
FVector AEnemyAIController::GetPlayerLocation()
{
	if (EnemyOwner)
		return EnemyOwner->GetPlayerReference()->GetActorLocation();
	else
		return FVector(0.0f);
}

// Set the enemy to chase the player
void AEnemyAIController::ChasePlayer()
{	
	if (EnemyOwner->GetCombatState() == ECombatState::COMBAT_Neutral)
	{
		GetPathFollowingComponent()->SetActive(true, true);
		
		EnemyOwner->UpdateMovementSpeed(EnemyOwner->RunSpeed); 

		MoveToActor(EnemyOwner->GetPlayerReference(), EnemyOwner->GetAttackRadius() - 100.0f, true, true, true, 0, true);
	}
}

// Set the enemy to move to the following location 
void AEnemyAIController::Move(FVector Destination)
{
	if (EnemyOwner->GetCombatState() == ECombatState::COMBAT_Neutral)
	{
		EnemyOwner->UpdateMovementSpeed(EnemyOwner->WalkSpeed);

		MoveToLocation(Destination, 1.0f, true, true, true, true, 0, true);
	}
}

// Set the enemy to move in the following pattern
void AEnemyAIController::Patrol(FVector StartLocation)
{
	if (EnemyOwner->GetCombatState() == ECombatState::COMBAT_Neutral)
	{
		EnemyOwner->UpdateMovementSpeed(EnemyOwner->WalkSpeed);

		if (PatrolSelection == EnemyOwner->PatrolLocations.Num())
		{
			PatrolSelection = 0;
			MoveToLocation(StartLocation, 1.0f, true, true, true, true, 0, true);
		}
		else
		{
			MoveToLocation(EnemyOwner->PatrolLocations[PatrolSelection], 1.0f, true, true, true, true, 0, true);
			PatrolSelection++;
		}
	}
}

// Call functions to perform line of sight calculation towards player
bool AEnemyAIController::CanSeePlayer()
{
	if (EnemyOwner)
		return EnemyOwner->IsPlayerClose() && EnemyOwner->IsPlayerWithinView(false, EnemyOwner->MaxSightAngle) && !EnemyOwner->IsPlayerBlocked(); 
	else
		return false;
}

// Call functions to check if player is standing behind enemy
bool AEnemyAIController::CanSensePlayerBehind()
{
	if (EnemyOwner)
		return EnemyOwner->IsPlayerClose() && EnemyOwner->IsPlayerWithinView(true, 90.0f) && !EnemyOwner->IsPlayerBlocked();
	else
		return false;
}

// Call function to check if the player's position is higher than the enemy
bool AEnemyAIController::IsPlayerAbove()
{
	if (EnemyOwner)
		return EnemyOwner->IsPlayerHigherThanEnemy();
	else
		return false;
}

// Call function to check if player has died
bool AEnemyAIController::IsPlayerDead()
{
	if (EnemyOwner)
		return EnemyOwner->GetPlayerReference()->IsDead();
	else
		return false;
}

// Call function to check if the owning enemy had died
bool AEnemyAIController::IsSelfDead()
{
	if (EnemyOwner)
		return EnemyOwner->IsDead();
	else
		return false;
}

// Call function to check if the enemy is ready to attack
bool AEnemyAIController::CanAttack()
{
	if (EnemyOwner)
		return EnemyOwner->IsPlayerWithinAttackRadius() && EnemyOwner->GetCombatState() == ECombatState::COMBAT_Neutral; 
	else
		return false;
}

// Randomly decide whether to use this enemy's alternate attack or primary attack
bool AEnemyAIController::CanUseAlternateAttack()
{
	return ChanceOfAlternateAttack >= FMath::RandRange(0.0f, 1.0f);
}

// Call function to perform attack animation 
void AEnemyAIController::StartAttackAnimation(FAttackAnimation AttackAnim)
{
	if (EnemyOwner->GetCombatState() == ECombatState::COMBAT_Neutral)
		EnemyOwner->PlayAttackAnim(AttackAnim); 
}

