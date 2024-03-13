
#pragma once

#include "CoreMinimal.h"
#include "CombatAnimNotify.h"
#include "CombatAnimNotify_AttachRope.generated.h"

UCLASS()
class COMBATPRACTICE_API UCombatAnimNotify_AttachRope : public UCombatAnimNotify
{
	GENERATED_BODY()
	
public:
	// Set default values 
	UCombatAnimNotify_AttachRope();

protected:
	// Triggered when this anim notify is called
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	/** Set whether the rope will be attached to a grapple point or not */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bShouldAttach;
};
