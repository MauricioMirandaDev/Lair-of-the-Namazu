
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
	/** Set whether the character will start and stop moving at this point */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bStopMovement; 
};
