
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

	// Checks to see if character has run out of health
	bool IsDead();

	// This character's weapon 
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	AWeapon* Weapon;

	// AnimNotify that accesses CombatState
	friend class UCombatAnimNotify_ChangeState;

	// AnimNotify that calls ForwardThrust() 
	friend class UCombatAnimNotify_ForwardThrust; 

	// Freind class that accesses TakeDamage() and CurrentAttackAnimation
	friend class AWeapon; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when character runs out of health
	virtual void OnDeath();

	// This character's current state
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	ECombatState CombatState; 

	// The attack animation this character is currently performing
	FAttackAnimation CurrentAttackAnimation;

private: 
	// Components and functions used for combat
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> WeaponClass;

	void TakeDamage(FAttackAnimation AttackAnimation, FVector AttackLocation); 

	void ForwardThrust(); 

	// Components and variables for health system
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentHealth;
};
