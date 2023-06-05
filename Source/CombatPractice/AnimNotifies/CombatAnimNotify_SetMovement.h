
#pragma once

#include "CoreMinimal.h"
#include "CombatAnimNotify.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CombatAnimNotify_SetMovement.generated.h"

UCLASS()
class COMBATPRACTICE_API UCombatAnimNotify_SetMovement : public UCombatAnimNotify
{
	GENERATED_BODY()
	
public:
	// Set default values
	UCombatAnimNotify_SetMovement();

protected:
	// Triggered when this anim notify is called
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private: 
	// MovementMode that will be set during the attack animtion
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EMovementMode> NewMovement;  
};
