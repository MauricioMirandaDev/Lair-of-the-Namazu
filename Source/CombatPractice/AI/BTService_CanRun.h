
#pragma once

#include "CoreMinimal.h"
#include "BTService_Combat.h"
#include "BTService_CanRun.generated.h"

UCLASS()
class COMBATPRACTICE_API UBTService_CanRun : public UBTService_Combat
{
	GENERATED_BODY()
	
public:
	// Set default values
	UBTService_CanRun(); 

protected:
	// Update next tick
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
