
#pragma once

#include "CoreMinimal.h"
#include "CombatAnimNotify.h"
#include "CAN_SetAttackEffects.generated.h"

UCLASS()
class COMBATPRACTICE_API UCAN_SetAttackEffects : public UCombatAnimNotify
{
	GENERATED_BODY()
	
public:
	UCAN_SetAttackEffects(); 

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bStartOfAttack;
};
