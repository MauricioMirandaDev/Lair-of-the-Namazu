
#pragma once

#include "CoreMinimal.h"
#include "CombatCharacter.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class COMBATPRACTICE_API APlayerCharacter : public ACombatCharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Friend class that handles input bindings
	friend class ACombatPlayerController; 

	// AnimNotify that accesses bCanAttack
	friend class UCombatAnimNotify_SetAttack;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when character runs out of health
	virtual void OnDeath() override; 

private:
	// Components for camera control
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	// Components, functions, and variables for combat
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* LightAttackAnimation;

	void LightAttack(); 

	class ACombatPlayerController* ControllerRef; 

	bool bCanAttack;
};
