
#include "BTService_CanRun.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CombatPractice/AI/EnemyAIController.h"
#include "CombatPractice/Characters/EnemyCharacter.h"

UBTService_CanRun::UBTService_CanRun()
{
	NodeName = TEXT("Observe Combat State");
}

void UBTService_CanRun::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	switch (EnemyController->GetEnemyOwner()->GetCombatState())
	{
		case ECombatState::COMBAT_DamagedNormal:
		case ECombatState::COMBAT_DamagedHeavy:
		case ECombatState::COMBAT_DamagedStun:
			EnemyController->GetBrainComponent()->StopLogic("Damaged");
			break;
		default:
			EnemyController->GetBrainComponent()->StartLogic();
			break;
	}
}
