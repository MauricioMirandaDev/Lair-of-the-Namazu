
#include "BTService_CheckPlayerAlive.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CombatPractice/AI/EnemyAIController.h"
#include "CombatPractice/Characters/EnemyCharacter.h"
#include "CombatPractice/Characters/PlayerCharacter.h"

// Set default values
UBTService_CheckPlayerAlive::UBTService_CheckPlayerAlive()
{
	NodeName = TEXT("Check for Player Alive");
}

// Check to see if the player is still alive
void UBTService_CheckPlayerAlive::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (Enemy->GetPlayerReference()->IsDead())
		OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
	else
		OwnerComp.GetAIOwner()->GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
}
