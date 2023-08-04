
#pragma once

#include "CoreMinimal.h"
#include "BTTask_Combat.h"
#include "CombatPractice/Characters/CombatCharacter.h"
#include "BTTask_AttackAnimation.generated.h" 

UCLASS()
class COMBATPRACTICE_API UBTTask_AttackAnimation : public UBTTask_Combat
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
