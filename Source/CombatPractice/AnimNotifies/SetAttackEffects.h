
#pragma once

#include "CoreMinimal.h"
#include "CombatAnimNotify.h"
#include "SetAttackEffects.generated.h"

UCLASS()
class COMBATPRACTICE_API USetAttackEffects : public UCombatAnimNotify
{
	GENERATED_BODY()
	
public:
	// Set default values
	USetAttackEffects();

protected:
	// Triggered when this anim notify is called
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	// Variable to determine whehter this anim notify is being called at the start or end of an attack animation 
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bStartOfAttack;
};
