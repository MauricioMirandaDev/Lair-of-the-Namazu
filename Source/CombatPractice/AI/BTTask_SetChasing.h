
#pragma once

#include "CoreMinimal.h"
#include "BTTask_Combat.h"
#include "BTTask_SetChasing.generated.h"

UCLASS()
class COMBATPRACTICE_API UBTTask_SetChasing : public UBTTask_Combat
{
	GENERATED_BODY()
	
public:
	// Set default values
	UBTTask_SetChasing();

protected:
	// Starts task, returns succeeded or failed
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	// Flag to check if enemy is actively chasing player
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bIsActivelyChasing; 
};
