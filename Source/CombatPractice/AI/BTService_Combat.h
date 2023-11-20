
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_Combat.generated.h"

UCLASS()
class COMBATPRACTICE_API UBTService_Combat : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	// Set default values
	UBTService_Combat();

protected:
	// Update next tick
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// Reference to the controller performing this service 
	class AEnemyAIController* EnemyController;
};
