
#include "BTTask_ClearBlackboardKey.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CombatPractice/AI/EnemyAIController.h"

// Set default values
UBTTask_ClearBlackboardKey::UBTTask_ClearBlackboardKey()
{
	NodeName = TEXT("Clear Blackboard Key");
}

// Clear the value of the selected blackboard key
EBTNodeResult::Type UBTTask_ClearBlackboardKey::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetAIOwner()->GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());

	return EBTNodeResult::Succeeded;
}
