
#pragma once

#include "CoreMinimal.h"
#include "CombatCharacter.h"
#include "PlayerCharacter.generated.h" 

class AEnemyCharacter; 

UCLASS()
class COMBATPRACTICE_API APlayerCharacter : public ACombatCharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Friend class that handles input bindings
	friend class ACombatPlayerController; 

	// Friend class that access NearbyEnemies[] 
	friend class AEnemyCharacter; 

	// AnimNotify that accesses bCanAttack
	friend class UCombatAnimNotify_SetAttack;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Functions used for movement
	virtual void Jump() override; 

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override; 

	// Functions used for combat
	virtual void OnDeath() override; 

	virtual void ResetAttack() override;

private:
	// Components for camera control
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	// Components, functions, and variables for combat
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	FAttackAnimation LightAttack_Phase01; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	FAttackAnimation LightAttack_Phase02; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	FAttackAnimation LightAttack_Phase03;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	FAttackAnimation LightAttack_Air;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	FAttackAnimation HeavyAttack;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bJumpPressed; 

	void LightAttackPressed(); 

	void HeavyAttackPressed(); 

	bool bCanAttack;

	int32 AttackCount;

	// Components, functions, and variables for lock-on system
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lock-On System", meta = (AllowPrivateAccess = "true"))
	float LockOnCameraOffset; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lock-On System", meta = (AllowPrivateAccess = "true"))
	float MaxLockOnDistance; 

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsLockedOn;

	void TraceForEnemies();

	AEnemyCharacter* DetermineClosestEnemy();

	void LockOntoEnemy();

	void LockedOnMovement();

	void LockOnBehavior();

	void SwitchEnemyUp();

	void SwitchEnemyDown();

	TArray<AEnemyCharacter*> NearbyEnemies;

	AEnemyCharacter* LockedOnEnemy;

	// Components for rope
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rope", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RopeHolster;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rope", meta = (AllowPrivateAccess = "true"))
	class UCableComponent* Rope;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rope", meta = (AllowPrivateAccess = "true"))
	float RopeLength; 
};
