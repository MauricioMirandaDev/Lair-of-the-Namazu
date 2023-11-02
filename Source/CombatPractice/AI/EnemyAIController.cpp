
#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CombatPractice/Characters/EnemyCharacter.h"

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

		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	}
}

void AEnemyAIController::MovementBehavior(FVector Destination)
{
	if (EnemyOwner->GetCombatState() == ECombatState::COMBAT_Neutral)
		MoveToLocation(Destination, 1.0f, true, true, false, true, 0, true);
}

// Called every frame
void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}


