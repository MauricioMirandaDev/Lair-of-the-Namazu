
#include "BTService_CheckPlayerAlive.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CombatPractice/AI/EnemyAIController.h"

// Set default values
UBTService_CheckPlayerAlive::UBTService_CheckPlayerAlive()
{
	NodeName = TEXT("Check for Alive Characters");
}

// Check to see if either the player has died or the owning enemy has died
void UBTService_CheckPlayerAlive::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (EnemyController->IsPlayerDead())
	{
		EnemyController->GetBlackboardComponent()->SetValueAsBool(IsPlayerDead.SelectedKeyName, true);
		EnemyController->GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}
	else if (EnemyController->IsSelfDead())
	{
		EnemyController->GetBlackboardComponent()->SetValueAsBool(IsSelfDead.SelectedKeyName, true);
		EnemyController->GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}
}
