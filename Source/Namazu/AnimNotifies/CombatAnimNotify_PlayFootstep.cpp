
#include "CombatAnimNotify_PlayFootstep.h"
#include "Namazu/Characters/CombatCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Set default values
UCombatAnimNotify_PlayFootstep::UCombatAnimNotify_PlayFootstep()
{
	NotifyColor = FColor(220, 220, 220);
	FootSocket = EFootSocket::SOCKET_LeftFoot;
	SoundEffect = nullptr; 
}

// Play footstep sound effect at the determined location
void UCombatAnimNotify_PlayFootstep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (OwnerReference && SoundEffect)
	{
		// Variables used for sphere trace
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(OwnerReference);

		FHitResult HitResult;

		// Perform a sphere trace to determine where to play the sound effect
		switch (FootSocket)
		{
		case EFootSocket::SOCKET_RightFoot:
			UKismetSystemLibrary::SphereTraceSingle(GetWorld(), OwnerReference->GetMesh()->GetSocketLocation(TEXT("RightFootSocket")), 
													OwnerReference->GetMesh()->GetSocketLocation(TEXT("RightFootSocket")), 50.0f, 
													UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore,
													EDrawDebugTrace::None, OUT HitResult, false);
			break;
		default:
			UKismetSystemLibrary::SphereTraceSingle(GetWorld(), OwnerReference->GetMesh()->GetSocketLocation(TEXT("LeftFootSocket")),
													OwnerReference->GetMesh()->GetSocketLocation(TEXT("LeftFootSocket")), 50.0f,
													UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore,
													EDrawDebugTrace::None, OUT HitResult, false);
			break;
		}

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundEffect, HitResult.Location);
	}
}
