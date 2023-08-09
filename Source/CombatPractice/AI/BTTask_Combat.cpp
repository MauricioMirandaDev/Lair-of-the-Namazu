
#include "BTTask_Combat.h"
#include "CombatPractice/AI/EnemyAIController.h"
#include "CombatPractice/Characters/EnemyCharacter.h"

// Set default values
UBTTask_Combat::UBTTask_Combat()
{
	NodeName = TEXT("Parent Task");
	Enemy = nullptr; 
}

// Get a reference to the enemy in game
EBTNodeResult::Type UBTTask_Combat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	Enemy = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Enemy == nullptr)
		return EBTNodeResult::Failed;

	return EBTNodeResult::Succeeded;
}
