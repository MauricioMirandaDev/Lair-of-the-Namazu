
#include "BTTask_SetChasing.h"
#include "BehaviorTree/BlackboardComponent.h"

// Set default values
UBTTask_SetChasing::UBTTask_SetChasing()
{
	NodeName = TEXT("Set IsChasing");
	bIsActivelyChasing = false; 
}

// Set whether the enemy is actively chasing the player or not
EBTNodeResult::Type UBTTask_SetChasing::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), bIsActivelyChasing);

	return EBTNodeResult::Type();
}
