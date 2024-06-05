
#pragma once

#include "CoreMinimal.h"
#include "BTTask_Combat.h"
#include "Namazu/Characters/AttackAnimation.h"
#include "BTTask_AttackAnimation.generated.h" 

UCLASS()
class NAMAZU_API UBTTask_AttackAnimation : public UBTTask_Combat
{
	GENERATED_BODY()
	
public:
	// Set default values
	UBTTask_AttackAnimation();

protected:
	// Starts task, returns succeeded or failed
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	// Attack animation for the enemy to perform
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	FAttackAnimation AttackAnimation;
};
