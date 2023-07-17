
#pragma once

#include "CoreMinimal.h"
#include "CombatAnimNotify.h"
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
	/** The updated walk speed value; set to 0.0 to stop movement */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float UpdatedMaxWalkSpeed; 
};
