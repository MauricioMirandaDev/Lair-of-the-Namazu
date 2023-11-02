
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

UCLASS()
class COMBATPRACTICE_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	// Set default values for this AI controller
	AEnemyAIController();

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	void MovementBehavior(FVector Destination); 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// This enemy's behavior tree
	UPROPERTY(EditDefaultsOnly, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTree;

	class AEnemyCharacter* EnemyOwner; 
};
