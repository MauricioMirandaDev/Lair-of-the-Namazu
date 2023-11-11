
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "CombatPractice/Characters/EnemyCharacter.h"
#include "CombatPractice/Characters/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"

// Set default values
AEnemyAIController::AEnemyAIController()
{
	BehaviorTree = nullptr; 
	EnemyOwner = nullptr; 
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

		GetBlackboardComponent()->SetValueAsBool(TEXT("CanRunTree"), true); 
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	}
}

// Called every frame
void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

UBehaviorTree* AEnemyAIController::GetBehaviorTree()
{
	return BehaviorTree;
}

AEnemyCharacter* AEnemyAIController::GetEnemyOwner()
{
	return EnemyOwner;
}

FVector AEnemyAIController::GetPlayerLocation()
{
	if (EnemyOwner)
		return EnemyOwner->GetPlayerReference()->GetActorLocation();
	else
		return FVector(0.0f);
}

void AEnemyAIController::ChasePlayer()
{
	if (EnemyOwner->GetCombatState() == ECombatState::COMBAT_Neutral)
		MoveToActor(EnemyOwner->GetPlayerReference(), EnemyOwner->AttackRadius - 100.0f, false, true, true, 0, true);
}

void AEnemyAIController::Move(FVector Destination)
{
	if (EnemyOwner->GetCombatState() == ECombatState::COMBAT_Neutral)
		MoveToLocation(Destination, 1.0f, true, true, true, true, 0, true);
}

// Perform a line of sight calculation to determine if the enemy can see the player
bool AEnemyAIController::CanSeePlayer()
{
	if (EnemyOwner)
		return EnemyOwner->IsPlayerClose() && EnemyOwner->IsPlayerWithinView(false, EnemyOwner->MaxSightAngle) && !EnemyOwner->IsPlayerBlocked(); 
	else
		return false;
}

// Perform a check to see if the player is located behind the enemy
bool AEnemyAIController::CanSensePlayerBehind()
{
	if (EnemyOwner)
		return EnemyOwner->IsPlayerClose() && EnemyOwner->IsPlayerWithinView(true, 90.0f) && !EnemyOwner->IsPlayerBlocked();
	else
		return false;
}

bool AEnemyAIController::IsPlayerDead()
{
	if (EnemyOwner)
		return EnemyOwner->GetPlayerReference()->IsDead();
	else
		return false;
}

bool AEnemyAIController::CanAttack()
{
	if (EnemyOwner)
		return EnemyOwner->PlayerWithinAttackRadius(); 
	else
		return false;
}

void AEnemyAIController::StartAttackAnimation(FAttackAnimation AttackAnim)
{
	if (EnemyOwner->GetCombatState() == ECombatState::COMBAT_Neutral)
		EnemyOwner->PlayAttackAnim(AttackAnim); 
}

