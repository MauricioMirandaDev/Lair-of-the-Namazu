
#include "BTService_PlayerHigher.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Namazu/Controllers/EnemyAIController.h"

// Set default values
UBTService_PlayerHigher::UBTService_PlayerHigher()
{
	NodeName = TEXT("Check Player is Higher");
}

// Determine if the player is positioned higher than the enemy
void UBTService_PlayerHigher::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (EnemyController->IsPlayerAbove())
		EnemyController->GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
	else
		EnemyController->GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
}
