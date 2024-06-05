
#pragma once

#include "CoreMinimal.h"
#include "CombatAnimNotify.h"
#include "CombatAnimNotify_PlayFootstep.generated.h"

UENUM(BlueprintType)
enum class EFootSocket : uint8 {
	SOCKET_LeftFoot UMETA(DisplayName = "Left Foot Socket"),
	SOCKET_RightFoot UMETA(DisplayName = "Right Foot Socket")
};

UCLASS()
class NAMAZU_API UCombatAnimNotify_PlayFootstep : public UCombatAnimNotify
{
	GENERATED_BODY()
	
public:
	// Set default values
	UCombatAnimNotify_PlayFootstep();

protected:
	// Triggered when this anim notify is called
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:
	/** Select which foot will be making the sound effect */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	EFootSocket FootSocket;

	/** Desired footstep sound effect */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	class USoundBase* SoundEffect;
};
