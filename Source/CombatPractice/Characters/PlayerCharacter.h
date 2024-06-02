
#pragma once

#include "CoreMinimal.h"
#include "CombatCharacter.h"
#include "PlayerCharacter.generated.h" 

class AEnemyCharacter;
class ARope;
class UAnimMontage; 

UENUM(BlueprintType)
enum class EPlayerFocus : uint8 {
	FOCUS_None UMETA(DisplayName = "None"),
	FOCUS_Enemy UMETA(DisplayName = "Enemy"),
	FOCUS_Rope UMETA(DisplayName = "Attached Point")
};

UCLASS()
class COMBATPRACTICE_API APlayerCharacter : public ACombatCharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override; 

	// Setter functions
	void SetCanGrapple(bool bCanPlayerGrapple);

	// Functions and variables used for combat
	virtual void UpdateMovement(bool bPauseMovement) override;

	virtual void ResetAttack() override;

	UFUNCTION(BlueprintPure)
	const int32 GetMedicineCount(); 

	UFUNCTION(BlueprintPure)
	const int32 GetKunaiCount();

	static int32 MedicineCount;

	static int32 KunaiCount; 

	// Functions used for health system 
	virtual void TakeDamage(FAttackAnimation AttackAnimation, FVector AttackLocation) override;

	virtual void AfterDeath() override; 

	// Function used for lock on system
	void EnemyDefeated(AActor* Enemy); 

	// Functions and variable used for rope
	UFUNCTION(BlueprintImplementableEvent)
	void Grapple(); 

	UFUNCTION(BlueprintPure)
	const int32 GetRopeCount();

	void AttachRope(); 

	static int32 RopeCount;

	// Friend class that handles input bindings
	friend class ACombatPlayerController; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Functions used for movement
	virtual void Jump() override; 

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override; 

	// Functions used for health system 
	virtual void OnDeath() override; 

	virtual void OnFallDeath() override; 

private:
	// Components for camera control
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	// Components, functions, and variables for combat
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	FAttackAnimation LightAttack; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> LightAttackVariations; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	FAttackAnimation JumpAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	FAttackAnimation HeavyAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	FAttackAnimation InstantAttack_Behind; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	FAttackAnimation InstantAttack_Ground;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float HealingAmount; 

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EPlayerFocus PlayerFocus;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bJumpPressed; 

	void LightAttackPressed(); 

	void HeavyAttackPressed(); 

	void InstantAttackPressed(); 

	void ConsumeMedicine(); 

	float DefaultWalkSpeed; 

	// Components, functions, and variables for lock-on system
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lock-On System", meta = (AllowPrivateAccess = "true"))
	float LockOnCameraOffset; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lock-On System", meta = (AllowPrivateAccess = "true"))
	float MaxLockOnDistance; 

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsLockedOn;

	void LockOn();

	AEnemyCharacter* FindClosestEnemy();

	void LockOnBehavior();

	void BeginLockingOn(); 

	void StopLockingOn(); 

	void LockedOnMovement();

	AEnemyCharacter* FindNearbyEnemy(FVector Direction);

	void SwitchLockedOnEnemy(FVector Direction); 

	AEnemyCharacter* LockedOnEnemy;

	bool bEnemyJustDefeated; 

	// Components, functions, and variables for grapple system
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rope", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ARope> RopeClass;

	UPROPERTY(BlueprintReadOnly, Category = "Rope", meta = (AllowPrivateAccess = "true"))
	ARope* Rope;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	FAttackAnimation LaunchAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	FAttackAnimation TripAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rope", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* CastRopeAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rope", meta = (AllowPrivateAccess = "true"))
	float TensionStrength;

	UFUNCTION(BlueprintCallable)
	void PrepareGrapple(); 

	UFUNCTION(BlueprintCallable)
	void FinishGrapple();

	UFUNCTION(BlueprintCallable)
	void LerpPlayerPosition(float Alpha);

	UFUNCTION(BlueprintCallable)
	void LaunchEnemy(AEnemyCharacter* DamagedEnemy); 

	UFUNCTION(BlueprintCallable)
	void ConsumeRope();

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float AngleToAttached;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsGrappling; 

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bRopeAttached; 

	void CastRope(); 

	void AttachedMovement(); 

	void AddTensionForce(); 

	void ReelIn(); 

	AEnemyCharacter* PreviousEnemyLaunched; 

	FVector InitialPosition;

	FVector EndPosition; 

	bool bCanGrapple; 
};
