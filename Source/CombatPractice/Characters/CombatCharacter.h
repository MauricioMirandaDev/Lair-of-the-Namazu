
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatCharacter.generated.h"

class AWeapon; 

UCLASS()
class COMBATPRACTICE_API ACombatCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACombatCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Anim notify that access AttackEffects(bIsStart)
	friend class USetAttackEffects; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Subclass used for spawning weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> WeaponClass;

	// Whether this character can attack or not
	bool bCanAttack; 

private: 
	// Sets variables during attack animations
	void AttackEffects(bool bIsStart);

	// This character's weapon 
	AWeapon* Weapon;
};
