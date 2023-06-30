
#pragma once

#include "CoreMinimal.h"
#include "CombatAnimNotify.h"
#include "CombatAnimNotify_ActivateHitbox.generated.h"


UCLASS()
class COMBATPRACTICE_API UCombatAnimNotify_ActivateHitbox : public UCombatAnimNotify
{
	GENERATED_BODY()
	
public:
	// Set default values
	UCombatAnimNotify_ActivateHitbox();

protected:
	// Triggered when this anim notify is called
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	/** The updated collision preset for this weapon's hitbox*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FName CollisionProfile;
};
