
#pragma once

#include "CoreMinimal.h"
#include "CombatAnimNotify.h"
#include "CombatAnimNotify_ForwardThrust.generated.h"

UCLASS()
class COMBATPRACTICE_API UCombatAnimNotify_ForwardThrust : public UCombatAnimNotify
{
	GENERATED_BODY()
	
public:
	// Set default values
	UCombatAnimNotify_ForwardThrust();

protected:
	// Triggered when this anim notify is called
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	// Determines how far the character is launched when performing a forward thrust
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float ThrustStrength; 
};
