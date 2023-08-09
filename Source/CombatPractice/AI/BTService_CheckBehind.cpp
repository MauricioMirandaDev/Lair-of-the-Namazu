
#include "BTService_CheckBehind.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CombatPractice/AI/EnemyAIController.h"
#include "CombatPractice/Characters/EnemyCharacter.h"
#include "CombatPractice/Characters/PlayerCharacter.h"

// Set default values
UBTService_CheckBehind::UBTService_CheckBehind()
{
	NodeName = TEXT("Check For Player Behind");
}

// If sight to the player is lost, check to see if the player is standing behind the enemy
void UBTService_CheckBehind::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (Enemy->PlayerReference)
	{
		if (FVector::DotProduct(Enemy->GetActorForwardVector(), Enemy->PlayerReference->GetActorLocation() - Enemy->GetActorLocation()) < 0.0f
			&& Enemy->IsPlayerBlocked() != true)
			OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Enemy->PlayerReference->GetActorLocation());
		else
			OwnerComp.GetAIOwner()->GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}
}
