
#pragma once

#include "CoreMinimal.h"
#include "BTService_SearchForPlayer.h"
#include "BTService_CheckBehind.generated.h"

UCLASS()
class NAMAZU_API UBTService_CheckBehind : public UBTService_SearchForPlayer
{
	GENERATED_BODY()
	
public:
	// Set default values
	UBTService_CheckBehind();

protected:
	// Update next tick
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
