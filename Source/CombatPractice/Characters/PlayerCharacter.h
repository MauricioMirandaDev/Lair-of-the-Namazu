
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

	// AnimNotify that accesses AttackCount
	friend class UCombatAnimNotify_ResetAttack; 

	// AnimNotify that accesses bCanAttack
	friend class UCombatAnimNotify_SetAttack;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called whenever the player performs a jump 
	virtual void Jump() override; 

	// Called when the player's movement mode changes
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override; 

	// Called when character runs out of health
	virtual void OnDeath() override; 

private:
	// Components for camera control
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	// Components, functions, and variables for combat
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	FAttackAnimation LightAttack_Phase01; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	FAttackAnimation LightAttack_Phase02; 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	FAttackAnimation LightAttack_Phase03;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	FAttackAnimation LightAttack_Air;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations", meta = (AllowPrivateAccess = "true"))
	FAttackAnimation HeavyAttack;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bJumpPressed; 

	void LightAttackPressed(); 

	void HeavyAttackPressed(); 

	class ACombatPlayerController* ControllerRef; 

	bool bCanAttack;

	int32 AttackCount;
};
