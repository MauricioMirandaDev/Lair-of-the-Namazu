
#pragma once

#include "CoreMinimal.h"
#include "CombatCharacter.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class COMBATPRACTICE_API AEnemyCharacter : public ACombatCharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// BehaviorTree Service that accesses CanSeePlayer() and PlayerReference
	friend class UBTService_SearchForPlayer; 

	// BehaviorTree Service that accesses PlayerReference 
	friend class UBTService_UpdatePlayerLocation;

	// BehaviorTree Service that accesses IsPlayerBlocked() and PlayerReference
	friend class UBTService_CheckBehind;

	// BehaviorTree Service that accesses IsReadytoAttack() 
	friend class UBTService_DetermineAttack; 

	// BehaviorTree Service that accesses PlayerReference
	friend class UBTService_CheckPlayerAlive;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when character runs out of health
	virtual void OnDeath() override; 

private:
	// Components, functions, and variable used for line of sight search 
	UPROPERTY(EditAnywhere, Category = "Search", meta = (AllowPrivateAccess = "true"))
	float SearchRadius;

	/** Should be between 0.0 and 180.0 */
	UPROPERTY(EditAnywhere, Category = "Search", meta = (AllowPrivateAccess = "true", UIMin = "0.0", UIMax = "180.0"))
	float MaxSightAngle;

	bool CanSeePlayer();

	bool IsPlayerClose(); 

	bool IsPlayerBlocked();

	class APlayerCharacter* PlayerReference;

	// Components, functions, and variables used for combat
	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float AttackRadius;

	bool IsReadyToAttack();
};
