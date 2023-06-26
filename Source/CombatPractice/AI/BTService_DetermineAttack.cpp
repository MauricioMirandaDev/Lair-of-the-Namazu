
#include "BTService_DetermineAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CombatPractice/AI/EnemyAIController.h"
#include "CombatPractice/Characters/EnemyCharacter.h"

// Set default values
UBTService_DetermineAttack::UBTService_DetermineAttack()
{
	NodeName = TEXT("Determine Attack");
}

// Determine if the enemy is close enough to attack the player
void UBTService_DetermineAttack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (OwningEnemy->PlayerReference && OwningEnemy->IsReadyToAttack())
		OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
	else
		OwnerComp.GetAIOwner()->GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
}