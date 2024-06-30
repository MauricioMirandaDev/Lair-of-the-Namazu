
#include "BTTask_Patrol.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Namazu/Controllers/EnemyAIController.h"

// Set default values
UBTTask_Patrol::UBTTask_Patrol()
{
	NodeName = TEXT("Patrol");
}

// Move the enemy in a patrol pattern, return to start after visiting all patrol points
EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (EnemyController)
	{
		EnemyController->Patrol(EnemyController->GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey()));

		return EBTNodeResult::Succeeded;
	}
	else
		return EBTNodeResult::Failed;
}
