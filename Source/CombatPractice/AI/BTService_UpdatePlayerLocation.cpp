
#include "BTService_UpdatePlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CombatPractice/AI/EnemyAIController.h"
#include "CombatPractice/Characters/EnemyCharacter.h"
#include "CombatPractice/Characters/PlayerCharacter.h"

// Set default values
UBTService_UpdatePlayerLocation::UBTService_UpdatePlayerLocation()
{
	NodeName = TEXT("Update Player Last Known Location");
}

// Update the player's last known location while chasing
void UBTService_UpdatePlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (OwningEnemy->PlayerReference)
		OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), OwningEnemy->PlayerReference->GetActorLocation());
}
