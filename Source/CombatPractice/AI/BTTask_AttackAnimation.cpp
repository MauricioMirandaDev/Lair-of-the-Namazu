
#include "BTTask_AttackAnimation.h"
#include "CombatPractice/Characters/EnemyCharacter.h"

// Set default values
UBTTask_AttackAnimation::UBTTask_AttackAnimation()
{
	NodeName = TEXT("Play Attack Animation");
}

// Play attack animation
EBTNodeResult::Type UBTTask_AttackAnimation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (Enemy && AttackAnimation)
		Enemy->PlayAnimMontage(AttackAnimation, 1.0f, TEXT("None"));

	return EBTNodeResult::Succeeded;
}
