
#include "BTService_ChanceAlternate.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Namazu/Controllers/EnemyAIController.h"

// Set default values
UBTService_ChanceAlternate::UBTService_ChanceAlternate()
{
	NodeName = TEXT("Chance of Alternate Attack");
}

// Use random chance to decide if this enemy will use their alternate attack
void UBTService_ChanceAlternate::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (EnemyController->CanUseAlternateAttack())
		EnemyController->GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
	else
		EnemyController->GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
}
