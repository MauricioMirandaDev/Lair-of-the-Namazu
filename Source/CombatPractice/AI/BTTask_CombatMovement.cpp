
#include "BTTask_CombatMovement.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CombatPractice/AI/EnemyAIController.h"
#include "CombatPractice/Characters/EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_CombatMovement::UBTTask_CombatMovement()
{
	NodeName = TEXT("Movement");
}

EBTNodeResult::Type UBTTask_CombatMovement::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (Enemy->GetCombatState() == ECombatState::COMBAT_Neutral)
	{
		OwnerComp.GetAIOwner()->MoveToLocation(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsVector(GetSelectedBlackboardKey()),
			1.0f, true, true, false, true, 0, true);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("STOPPING MOVEMENT"));
		//OwnerComp.GetAIOwner()->GetPathFollowingComponent()->SetActive(false, true);
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}
