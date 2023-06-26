
#pragma once

#include "CoreMinimal.h"
#include "CombatAnimNotify.h"
#include "CombatAnimNotify_RotateWeapon.generated.h"

UCLASS()
class COMBATPRACTICE_API UCombatAnimNotify_RotateWeapon : public UCombatAnimNotify
{
	GENERATED_BODY()
	
public:
	// Set default values
	UCombatAnimNotify_RotateWeapon();

protected:
	// Triggered when this anim notify is called
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	// The weapon's updated local rotation 
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FRotator NewRotation;
};
