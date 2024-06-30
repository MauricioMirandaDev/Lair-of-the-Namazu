
#pragma once

#include "CoreMinimal.h"
#include "CombatCharacter.h"
#include "NativeGameplayTags.h"
#include "PlayerCharacter.generated.h" 

class AEnemyCharacter;
class ARope;
class APickup; 
class UAnimMontage; 
class ULockOnComponent;
class UGrappleComponent;

UENUM(BlueprintType)
enum class EPlayerFocus : uint8 {
	FOCUS_None UMETA(DisplayName = "None"),
	FOCUS_Enemy UMETA(DisplayName = "Enemy"),
	FOCUS_Rope UMETA(DisplayName = "Attached Point")
};

UCLASS()
class NAMAZU_API APlayerCharacter : public ACombatCharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override; 

	// Getter functions
	UFUNCTION(BlueprintPure)
	const int32 GetMaxMedicine();

	UFUNCTION(BlueprintPure)
	const int32 GetMedicineCount(); 

	UFUNCTION(BlueprintPure)
	const int32 GetMaxKunai();

	UFUNCTION(BlueprintPure)
	const int32 GetKunaiCount();

	UFUNCTION(BlueprintPure)
	const int32 GetMaxRope();

	UFUNCTION(BlueprintPure)
	const int32 GetRopeCount();

	ULockOnComponent* GetLockOnComponent(); 

	UGrappleComponent* GetGrappleComponent(); 

	// Functions and variables used for combat
	static TMap<FGameplayTag, APickup*> CollectedPickups; 

	static int32 MaxKunai;

	static int32 KunaiCount;

	virtual void StopMovement(bool bPauseMovement) override;

	virtual void ResetAttack() override;

	void EnemyDefeated(AActor* Enemy);  

	// Functions and variables used for health system 
	static int32 MaxMedicine;

	static int32 MedicineCount;

	virtual void TakeDamage(FAttackAnimation AttackAnimation, FVector AttackLocation) override;

	virtual void AfterDeath() override; 

	// Functions and variable used for grapple system
	UFUNCTION(BlueprintImplementableEvent)
	void Grapple(); 

	void Attach(); 

	static int32 MaxRope;

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

	void ConsumeKunai(FAttackAnimation Animation, AEnemyCharacter* Enemy);

	void SelectFocusBehavior();

	bool bEnemyJustDefeated; 

	// Component and functions for lock-on system
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	ULockOnComponent* LockOnComponent; 

	void LockOn();

	void BeginLockingOn();

	void StopLockingOn();

	void SwitchLockedOnEnemy(FVector Direction); 

	// Components, functions, and variables for grapple system
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UGrappleComponent* GrappleComponent; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	FAttackAnimation LaunchAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	FAttackAnimation TripAttack;

	UFUNCTION(BlueprintCallable)
	void PrepareGrapple(); 

	UFUNCTION(BlueprintCallable)
	void ResetRope();

	UFUNCTION(BlueprintCallable)
	void LerpPlayerPosition(float Alpha);

	UFUNCTION(BlueprintCallable)
	void LaunchEnemy(AEnemyCharacter* DamagedEnemy); 

	UFUNCTION(BlueprintCallable)
	void ConsumeRope();

	void CastRod(); 

	void ReelIn(); 

	AEnemyCharacter* PreviousEnemyLaunched; 

	FVector InitialPosition;

	FVector EndPosition; 
};
