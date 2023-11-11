
#include "BTService_Combat.h"
#include "CombatPractice/AI/EnemyAIController.h"
#include "CombatPractice/Characters/EnemyCharacter.h"

// Set default values
UBTService_Combat::UBTService_Combat()
{
	NodeName = TEXT("Parent Service");
	Enemy = nullptr;
}

// Get a reference to the enemy in game
void UBTService_Combat::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	Enemy = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Enemy == nullptr)
		return; 

	EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (EnemyController == nullptr)
		return; 
}
