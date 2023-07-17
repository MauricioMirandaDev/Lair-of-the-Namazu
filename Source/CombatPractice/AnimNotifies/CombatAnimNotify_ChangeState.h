
#pragma once

#include "CoreMinimal.h"
#include "CombatAnimNotify.h"
#include "CombatPractice/Characters/CombatCharacter.h"
#include "CombatAnimNotify_ChangeState.generated.h"

UCLASS()
class COMBATPRACTICE_API UCombatAnimNotify_ChangeState : public UCombatAnimNotify
{
	GENERATED_BODY()

public:
	// Set default values
	UCombatAnimNotify_ChangeState();

protected:
	// Triggered when this anim notify is called
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	/** The updated state this character should be in*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ECombatState> NewState; 
};
