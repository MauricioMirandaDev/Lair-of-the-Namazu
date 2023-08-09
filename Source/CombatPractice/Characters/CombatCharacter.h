
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

	// This character's weapon 
	UPROPERTY(BlueprintReadOnly)
	AWeapon* Weapon;

	// Friend class that accesses TakeDamage() and CurrentAttackAnimation
	friend class AWeapon;

	// BehaviorTreee Service that accesses IsDead()
	friend class UBTService_CheckPlayerAlive;

	// AnimNotify that accesses CombatState
	friend class UCombatAnimNotify_ChangeState;

	// AnimNotify that calls ForwardThrust() 
	friend class UCombatAnimNotify_ForwardThrust; 

	// AnimNotify that calls ResetAttack()
	friend class UCombatAnimNotify_ResetAttack;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Components, functions, and variable used for combat
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ECombatState CombatState; 

	virtual void OnDeath();

	virtual void ResetAttack();

	FAttackAnimation CurrentAttackAnimation;

	// Function used for health system
	bool IsDead();

private: 
	// Component and function used for combat
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> WeaponClass;

	void ForwardThrust(); 

	// Components and function used for health system
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentHealth;

	void TakeDamage(FAttackAnimation AttackAnimation, FVector AttackLocation);
};
