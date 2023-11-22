
#include "BTService_DetermineAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CombatPractice/Controllers/EnemyAIController.h"

// Set default values
UBTService_DetermineAttack::UBTService_DetermineAttack()
{
	NodeName = TEXT("Determine Attack");
}

// Determine if the enemy is close enough to attack the player
void UBTService_DetermineAttack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (EnemyController->CanAttack() && !EnemyController->IsPlayerDead())
		EnemyController->GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
	else
		EnemyController->GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey()); 
}