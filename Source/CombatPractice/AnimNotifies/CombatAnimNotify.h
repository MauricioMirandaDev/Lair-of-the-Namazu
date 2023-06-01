
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CombatAnimNotify.generated.h"

class ABaseCharacter; 

UCLASS()
class COMBATPRACTICE_API UCombatAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	UCombatAnimNotify();

protected: 
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	ABaseCharacter* PlayerRef;
};
