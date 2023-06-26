
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

	// Anim notify that accesses bCanAttack
	friend class UCombatAnimNotify_SetAttack; 

	// Anim notify that accesses Weapon
	friend class UCombatAnimNotify_RotateWeapon;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Subclass used for spawning weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> WeaponClass;

	// Whether this character can attack or not
	bool bCanAttack; 

private: 
	// This character's weapon 
	AWeapon* Weapon;
};
