
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

	if (Enemy->GetCombatState() == ECombatState::COMBAT_Neutral)
		OwnerComp.GetAIOwner()->GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
	else
		OwnerComp.GetAIOwner()->GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey()); 
}
