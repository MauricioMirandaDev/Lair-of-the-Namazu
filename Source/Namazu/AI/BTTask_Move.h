
#pragma once

#include "CoreMinimal.h"
#include "BTTask_Combat.h"
#include "BTTask_Move.generated.h"

UCLASS()
class NAMAZU_API UBTTask_Move : public UBTTask_Combat
{
	GENERATED_BODY()
	
public:
	// Set default values
	UBTTask_Move();

protected:
	// Starts task, returns succeeded or failed
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
