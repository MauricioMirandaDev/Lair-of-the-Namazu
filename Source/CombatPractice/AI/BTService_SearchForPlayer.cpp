
#include "BTService_SearchForPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CombatPractice/AI/EnemyAIController.h"

// Set default values
UBTService_SearchForPlayer::UBTService_SearchForPlayer()
{
	NodeName = TEXT("Search for Player");
}

// Determine if the owning enemy can see the player or not
void UBTService_SearchForPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (EnemyController->CanSeePlayer())
		EnemyController->GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true); 
	else
		EnemyController->GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
}
