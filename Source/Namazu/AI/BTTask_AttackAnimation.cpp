
#include "BTTask_AttackAnimation.h"
#include "Namazu/Controllers/EnemyAIController.h"

// Set default values
UBTTask_AttackAnimation::UBTTask_AttackAnimation()
{
	NodeName = TEXT("Play Attack Animation");
}

// Set the enemy to play the following attack animation 
EBTNodeResult::Type UBTTask_AttackAnimation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (EnemyController)
	{
		EnemyController->StartAttackAnimation(AttackAnimation);

		return EBTNodeResult::Succeeded;
	}
	else
		return EBTNodeResult::Failed;
}
