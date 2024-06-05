
#pragma once

#include "CoreMinimal.h"
#include "BTTask_Combat.h"
#include "BTTask_ChasePlayer.generated.h"

UCLASS()
class NAMAZU_API UBTTask_ChasePlayer : public UBTTask_Combat
{
	GENERATED_BODY()
	
public:
	UBTTask_ChasePlayer();

protected:
	// Starts task, returns succeeded or failed
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
