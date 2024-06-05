
#pragma once

#include "CoreMinimal.h"
#include "CombatAnimNotify.h"
#include "CombatAnimNotify_ResetAttack.generated.h"

UCLASS()
class NAMAZU_API UCombatAnimNotify_ResetAttack : public UCombatAnimNotify
{
	GENERATED_BODY()
	
public:
	// Set default values
	UCombatAnimNotify_ResetAttack();

protected:
	// Triggered when this anim notify is called
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
