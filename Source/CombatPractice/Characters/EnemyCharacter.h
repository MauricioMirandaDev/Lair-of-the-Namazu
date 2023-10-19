
#pragma once

#include "CoreMinimal.h"
#include "CombatCharacter.h"
#include "EnemyCharacter.generated.h"

class UWidgetComponent;
class APlayerCharacter;

UCLASS()
class COMBATPRACTICE_API AEnemyCharacter : public ACombatCharacter
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

	// Functions used for line of sight search
	bool CanSeePlayer();

	bool IsPlayerBlocked();

	// Functions used for combat
	virtual void ResetAttack() override; 

	bool IsReadyToAttack();

	// Function used for health system
	void AfterDeathEffects();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Function used for combat
	virtual void OnDeath() override; 

private:
	// Components, function, and variable used for line of sight search 
	UPROPERTY(EditAnywhere, Category = "Search", meta = (AllowPrivateAccess = "true"))
	float SearchRadius;

	/** Should be between 0.0 and 180.0 */
	UPROPERTY(EditAnywhere, Category = "Search", meta = (AllowPrivateAccess = "true", UIMin = "0.0", UIMax = "180.0"))
	float MaxSightAngle;

	bool IsPlayerClose(); 

	APlayerCharacter* PlayerReference;

	// Components and variable used for combat
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* HealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* LockOnTarget;

	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float AttackRadius;
};
