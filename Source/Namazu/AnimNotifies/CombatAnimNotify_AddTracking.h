
#pragma once

#include "CoreMinimal.h"
#include "CombatAnimNotify.h"
#include "CombatAnimNotify_AddTracking.generated.h"

UCLASS()
class NAMAZU_API UCombatAnimNotify_AddTracking : public UCombatAnimNotify
{
	GENERATED_BODY()
	
public:
	// Sets default values
	UCombatAnimNotify_AddTracking();

protected:
	// Triggered when this anim notify is called
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
