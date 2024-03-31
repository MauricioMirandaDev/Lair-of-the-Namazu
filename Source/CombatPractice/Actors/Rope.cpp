
#include "Rope.h"
#include "CableComponent.h"
#include "CombatPractice/Actors/GrapplePoint.h"
#include "CombatPractice/Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ARope::ARope()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TargetGrapplePoint = nullptr; 

	// Create scene component and set as root
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// Create cable component and set default values
	Cable = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
	Cable->SetupAttachment(Root);
	Cable->bAttachStart = true;
	Cable->bAttachEnd = true;
	Cable->EndLocation = FVector(0.0f, 0.0f, 0.0f);
}

// Called when the game starts or when spawned
void ARope::BeginPlay()
{
	Super::BeginPlay();

	UpdateRopeAttached(false);
}

// Called every frame
void ARope::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DetermineClosestGrapplePoint(); 
}

// Get a reference to the target grapple point
AGrapplePoint* ARope::GetTargetPoint()
{
	return TargetGrapplePoint;
}

// Set reference to player
void ARope::SetPlayerReference(APlayerCharacter* Player)
{
	PlayerRef = Player;
}

// Attach the rope to a grapple point or detach it
void ARope::UpdateRopeAttached(bool bShouldAttach)
{
	Cable->SetVisibility(bShouldAttach);

	if (bShouldAttach)
		Cable->SetAttachEndToComponent(TargetGrapplePoint->GetRootComponent());
	else
		Cable->SetAttachEndToComponent(Root);
}

// Find the grapple point closest to where the player is looking
void ARope::DetermineClosestGrapplePoint()
{
	// Variables for trace
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel4));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(PlayerRef);

	TArray<AActor*> FoundActors;
	TArray<AGrapplePoint*> NearbyPoints;

	FVector CameraForward = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetActorForwardVector();

	FHitResult HitResult;

	// Find all nearby grapple points
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), Cable->CableLength, ObjectTypes, AGrapplePoint::StaticClass(), ActorsToIgnore, OUT FoundActors);

	// Determine which points are within line of site to the player
	for (AActor* Actor : FoundActors)
	{
		if (!UKismetSystemLibrary::LineTraceSingle(GetWorld(), PlayerRef->GetActorLocation(), Actor->GetActorLocation(), UEngineTypes::ConvertToTraceType(ECC_Visibility), 
												  true, ActorsToIgnore, EDrawDebugTrace::None, OUT HitResult, true))
			NearbyPoints.Add(Cast<AGrapplePoint>(Actor));
	}

	// Select the closest grapple point based on camera rotation
	if (!NearbyPoints.IsEmpty())
	{
		TargetGrapplePoint = NearbyPoints[0];
		TargetGrapplePoint->SetIconVisibility(false); 

		if (NearbyPoints.Num() > 1)
		{
			for (AGrapplePoint* NextPoint : NearbyPoints)
			{
				NextPoint->SetIconVisibility(false);

				FVector DirectionToNext = NextPoint->GetActorLocation() - PlayerRef->GetActorLocation();
				DirectionToNext.Normalize();

				FVector DirectionToTarget = TargetGrapplePoint->GetActorLocation() - PlayerRef->GetActorLocation();
				DirectionToTarget.Normalize();

				if (FVector::DotProduct(CameraForward, DirectionToNext) > FVector::DotProduct(CameraForward, DirectionToTarget))
					TargetGrapplePoint = NextPoint;
			}
		}

		TargetGrapplePoint->SetIconVisibility(true);
		PlayerRef->SetCanGrapple(true);
	}
	else
	{
		if (TargetGrapplePoint)
		{
			TargetGrapplePoint->SetIconVisibility(false);
			TargetGrapplePoint = nullptr;
		}

		PlayerRef->SetCanGrapple(false);
	}
}
