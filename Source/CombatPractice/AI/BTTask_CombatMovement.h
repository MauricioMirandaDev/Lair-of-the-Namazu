
#pragma once

#include "CoreMinimal.h"
#include "BTTask_Combat.h"
#include "BTTask_CombatMovement.generated.h"

UCLASS()
class COMBATPRACTICE_API UBTTask_CombatMovement : public UBTTask_Combat
{
	GENERATED_BODY()
	
public:
	// Set default values
	UBTTask_CombatMovement();

protected:
	// Starts task, returns succeeded or failed
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
