
#include "BTService_Combat.h"
#include "CombatPractice/Controllers/EnemyAIController.h"

// Set default values
UBTService_Combat::UBTService_Combat()
{
	NodeName = TEXT("Parent Service");
	EnemyController = nullptr;
}

// Get a reference to the enemy's controller
void UBTService_Combat::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (EnemyController == nullptr)
		return; 
}
