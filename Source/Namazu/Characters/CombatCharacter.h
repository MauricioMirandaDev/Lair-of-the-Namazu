
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Namazu/Characters/AttackAnimation.h"
#include "Namazu/Characters/CombatState.h"
#include "CombatCharacter.generated.h"

class AWeapon;

UCLASS()
class NAMAZU_API ACombatCharacter : public ACharacter
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
	virtual void StopMovement(bool bPauseMovement); 

	virtual void ResetAttack();

	void ForwardThrust();

	// Functions used for health system
	virtual	void TakeDamage(FAttackAnimation AttackAnimation, FVector AttackLocation);

	virtual void AfterDeath();

	bool IsDead();

	// Friend class that calls OnFallDeath()
	friend class AFallVolume;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Components, function, and variable used for combat
	UPROPERTY(BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	AWeapon* Weapon;

	UPROPERTY(BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	ECombatState CombatState; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float RunSpeed; 

	void PlayAttackAnim(FAttackAnimation AttackAnimation);

	void UpdateMovementSpeed(float NewSpeed);

	FAttackAnimation CurrentAttackAnimation;

	// Components and function used for health system 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentHealth;

	virtual void OnDeath();

	virtual void OnFallDeath(); 

private: 
	// Component used for combat
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* DamagedNormalMontage;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bLandedFromLaunch; 
};
