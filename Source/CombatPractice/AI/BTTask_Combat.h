
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Combat.generated.h"

UCLASS()
class COMBATPRACTICE_API UBTTask_Combat : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	// Set default values
	UBTTask_Combat();

protected:
	// Starts task, returns succeeded or failed
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// References to the player and owning enemy
	class APlayerCharacter* Player;

	class AEnemyCharacter* Enemy;
};
