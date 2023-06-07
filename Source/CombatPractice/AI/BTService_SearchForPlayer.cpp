
#include "BTService_SearchForPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CombatPractice//AI/EnemyAIController.h"
#include "CombatPractice/Characters/EnemyCharacter.h"
#include "CombatPractice/Characters/PlayerCharacter.h"

// Set default values
UBTService_SearchForPlayer::UBTService_SearchForPlayer()
{
	NodeName = TEXT("Search for Player");
}

// Determine if the player is in front of an enemy or behind 
void UBTService_SearchForPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (Player && Enemy)
	{
		float DotProd = FVector::DotProduct(Enemy->GetActorForwardVector(), Player->GetActorLocation() - Enemy->GetActorLocation());

		if (DotProd > 0.0f)
		{
			OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Player->GetActorLocation());
		}
		else
			OwnerComp.GetAIOwner()->GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}
}
