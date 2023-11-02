
#pragma once

#include "CoreMinimal.h"
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
	COMBAT_AttackRecover UMETA(DisplayName = "Attack Recovery")
};

// Type of attack the character is performing
UENUM(BlueprintType)
enum class EAttackType : uint8 {
	ATTACK_Normal UMETA(DisplayName = "Nomral Attack"),
	ATTACK_Heavy UMETA(DisplayName = "Heavy Attack"),
	ATTACK_Stun UMETA(DisplayName = "Stun Attack")
};

// Animation with relevant values for combat
USTRUCT(BlueprintType)
struct FAttackAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UAnimMontage* Animation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EAttackType AttackType; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ForwardThrustStrength; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KnockbackStrength;
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

	FAttackAnimation GetCurrentAttackAnim();

	// Setter functions
	void SetCombatState(ECombatState NewState);

	void SetCurrentAttackAnim(FAttackAnimation NewAnim);

	// Functions used for combat
	virtual void ResetAttack();

	void PlayAttackAnim(FAttackAnimation AttackAnimation);

	void ForwardThrust();

	// Functions used for health system
	virtual void AfterDeath();

	virtual	void TakeDamage(FAttackAnimation AttackAnimation, FVector AttackLocation);

	bool IsDead();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Components, function, and variable used for combat
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* DeathAnimation; 

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
