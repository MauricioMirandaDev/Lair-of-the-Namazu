
#include "BTService_Combat.h"
#include "CombatPractice/AI/EnemyAIController.h"
#include "CombatPractice/Characters/EnemyCharacter.h"

// Set default values
UBTService_Combat::UBTService_Combat()
{
	NodeName = TEXT("Parent Service");
	OwningEnemy = nullptr;
}

// Get a reference to the player and owning enemy
void UBTService_Combat::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	OwningEnemy = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (OwningEnemy == nullptr)
		return; 
}
