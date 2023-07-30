
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
	// Whether the hitbox will be turned on or off
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bActivateHitbox; 

	/** The type of damage that will be inflicted from this attack */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UCombatDamageType> DamageClass;
};
