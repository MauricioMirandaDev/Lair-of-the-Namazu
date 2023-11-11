
#pragma once

#include "CoreMinimal.h"
#include "CombatPractice/Characters/AttackAnimation.h"
#include "GameFramework/Character.h"
#include "CombatCharacter.generated.h"

class AWeapon;

// State the character is in
UENUM(BlueprintType)
enum class ECombatState : uint8 {
	COMBAT_Neutral UMETA(DisplayName = "Netural"),
	COMBAT_DamagedNormal UMETA(DisplayName = "Damaged Normal"), 
	COMBAT_DamagedHeavy UMETA(DisplayName = "Damaged Heavy"),
	COMBAT_DamagedStun UMETA(DisplayName = "Damaged Stun"),
	COMBAT_AttackStartup UMETA(DisplayName = "Attack Startup"),
	COMBAT_AttackActive UMETA(DisplayName = "Attack Active"),
	COMBAT_AttackRecover UMETA(DisplayName = "Attack Recovery"),
	COMBAT_Dead UMETA(DisplayName = "Dead")
};

UCLASS()
class COMBATPRACTICE_API ACombatCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACombatCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Getter functions
	AWeapon* GetWeapon();

	ECombatState GetCombatState(); 

	UFUNCTION(BlueprintCallable)
	FAttackAnimation GetCurrentAttackAnim();

	// Setter functions
	void SetCombatState(ECombatState NewState);

	void SetCurrentAttackAnim(FAttackAnimation NewAnim);

	// Functions used for combat
	virtual void SetMovement(bool bPauseMovement); 

	virtual void ResetAttack();

	void ForwardThrust();

	// Functions used for health system
	virtual void AfterDeath();

	virtual	void TakeDamage(FAttackAnimation AttackAnimation, FVector AttackLocation);

	UFUNCTION(BlueprintCallable)
	void PlayAttackAnim(FAttackAnimation AttackAnimation);

	bool IsDead();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Components, function, and variable used for combat
	UPROPERTY(BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	AWeapon* Weapon;

	UPROPERTY(BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	ECombatState CombatState; 

	virtual void OnDeath();

	FAttackAnimation CurrentAttackAnimation;

private: 
	// Component used for combat
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> WeaponClass;

	// Components used for health system
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentHealth;
};
