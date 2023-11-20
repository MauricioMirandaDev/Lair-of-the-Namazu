
#include "BTTask_Combat.h"
#include "CombatPractice/AI/EnemyAIController.h"

// Set default values
UBTTask_Combat::UBTTask_Combat()
{
	NodeName = TEXT("Parent Task");
	EnemyController = nullptr; 
}

// Get a reference to the enemy's controller 
EBTNodeResult::Type UBTTask_Combat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (EnemyController == nullptr)
		return EBTNodeResult::Failed;

	return EBTNodeResult::Succeeded;
}
