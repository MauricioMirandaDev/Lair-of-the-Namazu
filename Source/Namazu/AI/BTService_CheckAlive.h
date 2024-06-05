
#pragma once

#include "CoreMinimal.h"
#include "BTService_Combat.h"
#include "BTService_CheckAlive.generated.h"

UCLASS()
class NAMAZU_API UBTService_CheckPlayerAlive : public UBTService_Combat
{
	GENERATED_BODY()
	
public:
	// Set default values
	UBTService_CheckPlayerAlive();

protected:
	// Update next tick
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	// Blackboard keys 
	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector IsPlayerDead; 

	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector IsSelfDead;
};
