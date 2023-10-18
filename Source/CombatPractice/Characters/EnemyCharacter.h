
#pragma once

#include "CoreMinimal.h"
#include "CombatCharacter.h"
#include "EnemyCharacter.generated.h"

class UWidgetComponent;

UCLASS()
class COMBATPRACTICE_API AEnemyCharacter : public ACombatCharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UWidgetComponent* GetLockOnTarget();

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

	// BehaviorTree Task that accesses CurrentAttackAnimation
	friend class UBTTask_AttackAnimation;

	// AnimNotify that accesses AfterDeathEffects()
	friend class UCombatAnimNotify_AfterDeath;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Functions used for combat
	virtual void OnDeath() override; 

	virtual void ResetAttack() override; 

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

	// Component, function, and variable used for combat
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* HealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* LockOnTarget;

	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float AttackRadius;

	bool IsReadyToAttack();

	void AfterDeathEffects();
};
