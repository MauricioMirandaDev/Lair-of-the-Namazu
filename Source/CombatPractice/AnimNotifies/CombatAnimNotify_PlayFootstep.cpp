
#include "CombatAnimNotify_PlayFootstep.h"
#include "CombatPractice/Characters/CombatCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Set default values
UCombatAnimNotify_PlaySound3D::UCombatAnimNotify_PlaySound3D()
{
	NotifyColor = FColor(200, 200, 200);
	FootSocket = EFootSocket::SOCKET_LeftFoot;
	SoundEffect = nullptr; 
}

// Play a sound at the designated location
void UCombatAnimNotify_PlaySound3D::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (OwnerReference && SoundEffect)
	{
		// Variables used for sphere trace
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(OwnerReference);

		FHitResult HitResult;
		FVector TraceLocation;
		switch (FootSocket)
		{
		case EFootSocket::SOCKET_RightFoot:
			TraceLocation = OwnerReference->GetMesh()->GetSocketLocation(TEXT("RightFootSocket"));
			break;
		default:
			TraceLocation = OwnerReference->GetMesh()->GetSocketLocation(TEXT("LeftFootSocket"));
			break;
		}

		// Perform a sphere trace to get the location where the footstep should be played
		UKismetSystemLibrary::SphereTraceSingle(GetWorld(), TraceLocation, TraceLocation, 50.0f, UEngineTypes::ConvertToTraceType(ECC_Visibility), false,
			ActorsToIgnore, EDrawDebugTrace::None, OUT HitResult, true);

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundEffect, HitResult.Location, 1.0f, 1.0f, 0.0f);
	}
}
