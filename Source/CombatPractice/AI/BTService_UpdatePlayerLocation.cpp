
#include "BTService_UpdatePlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CombatPractice/Controllers/EnemyAIController.h"

// Set default values
UBTService_UpdatePlayerLocation::UBTService_UpdatePlayerLocation()
{
	NodeName = TEXT("Update Player Last Known Location");
}

// Update the player's last known location while chasing
void UBTService_UpdatePlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (EnemyController)
		EnemyController->GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), EnemyController->GetPlayerLocation());
}
