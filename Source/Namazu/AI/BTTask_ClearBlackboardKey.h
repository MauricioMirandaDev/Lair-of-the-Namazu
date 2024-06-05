
#pragma once

#include "CoreMinimal.h"
#include "BTTask_Combat.h"
#include "BTTask_ClearBlackboardKey.generated.h"

UCLASS()
class NAMAZU_API UBTTask_ClearBlackboardKey : public UBTTask_Combat
{
	GENERATED_BODY()
	
public:
	// Set default values
	UBTTask_ClearBlackboardKey();
	
protected:
	// Starts task, returns succeeded or failed
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
