
#pragma once

#include "CoreMinimal.h"
#include "CombatCharacter.h"
#include "CombatPractice/GrappleInterface.h"
#include "EnemyCharacter.generated.h"

class UWidgetComponent;
class APlayerCharacter;
class APickup; 

UCLASS()
class COMBATPRACTICE_API AEnemyCharacter : public ACombatCharacter, public IGrappleInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Getter functions
	APlayerCharacter* GetPlayerReference(); 

	UWidgetComponent* GetLockOnTarget();

	float GetAttackRadius(); 

	// Functions used for combat
	virtual void UpdateMovement(bool bPauseMovement) override;

	virtual void ResetAttack() override; 

	// Functions used for health system
	virtual void TakeDamage(FAttackAnimation AttackAnimation, FVector AttackLocation) override;

	virtual void AfterDeath() override; 

	// Interface function
	virtual FGrappleActor CreateGrappleActor() override;

	// Friend class that handles AI logic and actions
	friend class AEnemyAIController; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Functions used for health system
	virtual void OnDeath() override; 

	virtual void OnFallDeath() override; 

private:
	// Reference to this enemy's AI controller
	class AEnemyAIController* EnemyController;

	// Components, functions, and variable used for line of sight search 
	UPROPERTY(EditAnywhere, Category = "Search", meta = (AllowPrivateAccess = "true"))
	float SearchRadius;

	/** Should be between 0.0 and 180.0 */
	UPROPERTY(EditAnywhere, Category = "Search", meta = (AllowPrivateAccess = "true", UIMin = "0.0", UIMax = "180.0"))
	float MaxSightAngle;

	bool IsPlayerClose(); 

	bool IsPlayerWithinView(bool bCheckBehind, float Angle);

	bool IsPlayerBlocked();

	APlayerCharacter* PlayerReference;

	// Components and variable used for combat
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* HealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* LockOnIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickups", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<APickup>> PickupClasses;
	//TSubclassOf<APickup> PickupClass;

	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float AttackRadius;

	bool IsPlayerWithinAttackRadius(); 

	void SpawnPickup(); 

	// Components for grapple system
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* GrappleIcon;
};
