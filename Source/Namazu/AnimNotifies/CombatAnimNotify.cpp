
#include "CombatAnimNotify.h"
#include "Namazu/Characters/CombatCharacter.h"

// Set default values
UCombatAnimNotify::UCombatAnimNotify()
{
	OwnerReference = nullptr;
}

// Get a reference to the owning character
void UCombatAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (MeshComp && Animation)
	{
		OwnerReference = Cast<ACombatCharacter>(MeshComp->GetOwner());	

		if (OwnerReference == nullptr)
			return; 
	}
}