
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CombatAnimNotify.generated.h"

UCLASS()
class COMBATPRACTICE_API UCombatAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	// Set default values
	UCombatAnimNotify();

protected: 
	// Triggered when this anim notify is called
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	// Reference to this game's specific characters
	class ACombatCharacter* OwnerReference; 
};
