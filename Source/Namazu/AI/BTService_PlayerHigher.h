
#pragma once

#include "CoreMinimal.h"
#include "BTService_Combat.h"
#include "BTService_PlayerHigher.generated.h"

UCLASS()
class NAMAZU_API UBTService_PlayerHigher : public UBTService_Combat
{
	GENERATED_BODY()
	
public:
	// Set default values
	UBTService_PlayerHigher();

protected:
	// Update next tick
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
