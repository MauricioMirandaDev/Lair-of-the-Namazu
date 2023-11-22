
#include "BTTask_ChasePlayer.h"
#include "CombatPractice/Controllers/EnemyAIController.h"

// Set default values
UBTTask_ChasePlayer::UBTTask_ChasePlayer()
{
	NodeName = TEXT("Chase Player");
}

// Set the enemy to chase the player
EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (EnemyController)
	{
		EnemyController->ChasePlayer();

		return EBTNodeResult::Succeeded; 
	}
	else
		return EBTNodeResult::Failed;
}
