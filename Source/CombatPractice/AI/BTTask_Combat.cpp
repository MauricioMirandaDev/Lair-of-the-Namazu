
#include "BTTask_Combat.h"
#include "CombatPractice/AI/EnemyAIController.h"
#include "CombatPractice/Characters/EnemyCharacter.h"
#include "CombatPractice/Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Set default values
UBTTask_Combat::UBTTask_Combat()
{
	NodeName = TEXT("Parent Task");
	Player = nullptr;
	Enemy = nullptr; 
}

// Get a reference to the player and owning enemy
EBTNodeResult::Type UBTTask_Combat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player == nullptr)
		return EBTNodeResult::Failed;

	Enemy = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Enemy == nullptr)
		return EBTNodeResult::Failed;

	return EBTNodeResult::Succeeded;
}
