
#pragma once

#include "CoreMinimal.h"
#include "CombatAnimNotify.h"
#include "Namazu/Characters/GrappleComponent.h"
#include "CombatAnimNotify_UpdateRopeState.generated.h"

UCLASS()
class NAMAZU_API UCombatAnimNotify_UpdateRopeState : public UCombatAnimNotify
{
	GENERATED_BODY()
	
public:
	// Sets default values
	UCombatAnimNotify_UpdateRopeState(); 

protected:
	// Triggered when this anim notify is called
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	/** The updated state the rope should be in*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ERopeState> NewState;
};
