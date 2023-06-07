
#include "BTTask_TurnAround.h"
#include "CombatPractice/Characters/EnemyCharacter.h"

// Set default values
UBTTask_TurnAround::UBTTask_TurnAround()
{
	NodeName = TEXT("Turn Around");
}

// If sight of the player is lost, turn around to check if player is standing behind
EBTNodeResult::Type UBTTask_TurnAround::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (Enemy == nullptr)
		return EBTNodeResult::Failed;

	FVector InverseForward = Enemy->GetActorForwardVector() * -1.0f;
	Enemy->SetActorRotation(InverseForward.Rotation(), ETeleportType::None);

	return EBTNodeResult::Type();
}
