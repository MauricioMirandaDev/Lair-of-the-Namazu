
#include "BTTask_Move.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CombatPractice/Controllers/EnemyAIController.h"

// Set default values
UBTTask_Move::UBTTask_Move()
{
	NodeName = TEXT("Move To Location");
}

// Move the enemy to the desired location
EBTNodeResult::Type UBTTask_Move::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (EnemyController)
	{
		EnemyController->Move(EnemyController->GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey()));

		return EBTNodeResult::Succeeded;
	}
	else
		return EBTNodeResult::Failed;
}
