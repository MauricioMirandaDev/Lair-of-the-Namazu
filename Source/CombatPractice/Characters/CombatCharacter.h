
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
	COMBAT_DamagedHeavy UMETA(DisplayName = "Damaged Heavy")
};

// Type of attack the character is performing
UENUM(BlueprintType)
enum class EAttackType : uint8 {
	ATTACK_Normal UMETA(DisplayName = "Nomral Attack"),
	ATTACK_Heavy UMETA(DisplayName = "Heavy Attack")
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

	FAttackAnimation GetCurrentAttackAnim();

	// Setter functions
	void SetCombatState(ECombatState NewState);

	void SetCurrentAttackAnim(FAttackAnimation NewAnim);

	// Functions used for combat
	virtual void ResetAttack();

	void ForwardThrust();

	// Functions used for health system
	void TakeDamage(FAttackAnimation AttackAnimation, FVector AttackLocation);

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
