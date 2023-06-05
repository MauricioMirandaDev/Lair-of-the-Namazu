
#pragma once

#include "CoreMinimal.h"
#include "CombatAnimNotify.h"
#include "CombatAnimNotify_SetAttack.generated.h"

UCLASS()
class COMBATPRACTICE_API UCombatAnimNotify_SetAttack : public UCombatAnimNotify
{
	GENERATED_BODY()
	
public:
	// Set default values
	UCombatAnimNotify_SetAttack(); 

protected:
	// Triggered when this anim notify is called
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	// Whether the owning character will be able to start another attack or not 
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bAllowAttack; 
};
