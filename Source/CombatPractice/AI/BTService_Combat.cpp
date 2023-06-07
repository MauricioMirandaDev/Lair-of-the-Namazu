
#include "BTService_Combat.h"
#include "CombatPractice/AI/EnemyAIController.h"
#include "CombatPractice/Characters/EnemyCharacter.h"
#include "CombatPractice/Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Set default values
UBTService_Combat::UBTService_Combat()
{
	NodeName = TEXT("Parent Service");
	Player = nullptr;
	Enemy = nullptr;
}

// Get a reference to the player and owning enemy
void UBTService_Combat::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player == nullptr)
		return;
	
	Enemy = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Enemy == nullptr)
		return; 
}
