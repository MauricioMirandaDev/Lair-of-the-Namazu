
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Namazu/Characters/AttackAnimation.h"
#include "EnemyAIController.generated.h"

class AEnemyCharacter; 

UCLASS()
class NAMAZU_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	// Set default values for this AI controller
	AEnemyAIController();

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Getter functions
	AEnemyCharacter* GetEnemyOwner(); 

	FVector GetPlayerLocation(); 

	// Functions to handle enemy movement
	void ChasePlayer(); 

	void Move(FVector Destination); 

	void Patrol(FVector StartLocation);

	// Functions to handle enemy search
	bool CanSeePlayer();

	bool CanSensePlayerBehind(); 

	// Functions to handle combat
	bool IsPlayerDead(); 

	bool IsSelfDead(); 

	bool CanAttack(); 

	bool CanUseAlternateAttack();

	void StartAttackAnimation(FAttackAnimation AttackAnim); 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// This enemy's behavior tree
	UPROPERTY(EditDefaultsOnly, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTree;

	/** Enter a value between 0.0 and 1.0 */
	UPROPERTY(EditDefaultsOnly, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true", UIMin = "0.0", UIMax = "1.0"))
	float ChanceOfAlternateAttack;

	// Enemy who owns this controller 
	AEnemyCharacter* EnemyOwner;  

	// Variable to handle enemy movement
	int32 PatrolSelection; 
};
