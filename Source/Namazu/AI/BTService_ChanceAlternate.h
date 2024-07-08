
#pragma once

#include "CoreMinimal.h"
#include "BTService_Combat.h"
#include "BTService_ChanceAlternate.generated.h"

UCLASS()
class NAMAZU_API UBTService_ChanceAlternate : public UBTService_Combat
{
	GENERATED_BODY()
	
public:
	// Set default values
	UBTService_ChanceAlternate();

protected:
	// Update next tick
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
