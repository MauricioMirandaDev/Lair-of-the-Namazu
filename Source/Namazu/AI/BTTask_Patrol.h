
#pragma once

#include "CoreMinimal.h"
#include "BTTask_Combat.h"
#include "BTTask_Patrol.generated.h"

UCLASS()
class NAMAZU_API UBTTask_Patrol : public UBTTask_Combat
{
	GENERATED_BODY()
	
public:
	// Set default values
	UBTTask_Patrol();

protected:
	// Starts task, returns succeeded or failed
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
