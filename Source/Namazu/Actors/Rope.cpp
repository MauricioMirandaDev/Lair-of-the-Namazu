
#include "Rope.h"
#include "CableComponent.h"
#include "Namazu/Actors/GrapplePoint.h"
#include "Namazu/Characters/PlayerCharacter.h"
#include "Namazu/Characters/EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ARope::ARope()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Target = FGrappleActor(); 

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

	DetermineTarget(); 
}

// Access the target the rope is focussed on
FGrappleActor ARope::GetTarget()
{
	return Target;
}

// Set reference to player
void ARope::SetPlayerReference(APlayerCharacter* Player)
{
	PlayerRef = Player;
}

// Set the target to a new grapple actor
void ARope::SetTarget(FGrappleActor NewTarget)
{
	Target = NewTarget; 
	Target.SetIconVisibility(true); 
}

// Attach the rope to a grapple point or detach it
void ARope::UpdateRopeAttached(bool bShouldAttach)
{
	Cable->SetVisibility(bShouldAttach);

	if (bShouldAttach)
		Cable->SetAttachEndToComponent(Target.Actor->GetRootComponent());
	else
		Cable->SetAttachEndToComponent(Root);
}

// Find the grapple point closest to where the player is looking
void ARope::DetermineTarget()
{
	// Variables for trace
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel4));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(PlayerRef);

	TArray<AActor*> FoundActors;
	TArray<FGrappleActor> NearbyGrappleActors;

	FVector CameraForward = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetActorForwardVector();

	FHitResult HitResult;

	// Find all nearby actors that can be grappled onto
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), Cable->CableLength, ObjectTypes, nullptr, ActorsToIgnore, OUT FoundActors);

	// Determine which actors are within line of sight to the player
	for (AActor* Actor : FoundActors)
	{
		if (!UKismetSystemLibrary::LineTraceSingle(GetWorld(), PlayerRef->GetActorLocation(), Actor->GetActorLocation(), UEngineTypes::ConvertToTraceType(ECC_Visibility),
			true, ActorsToIgnore, EDrawDebugTrace::None, OUT HitResult, true)
			&&
			Actor->Implements<UGrappleInterface>())
		{
			if (AGrapplePoint* Point = Cast<AGrapplePoint>(Actor))
				NearbyGrappleActors.Add(Point->CreateGrappleActor());
			else if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Actor))
				NearbyGrappleActors.Add(Enemy->CreateGrappleActor()); 
		}
	}

	// Select the actor closest to the where the camera is pointed 
	if (!NearbyGrappleActors.IsEmpty())
	{
		Target = NearbyGrappleActors[0];
		Target.SetIconVisibility(false);

		if (NearbyGrappleActors.Num() > 1)
		{
			for (int i = 1; i <= NearbyGrappleActors.Num() - 1; i++)
			{
				NearbyGrappleActors[i].SetIconVisibility(false);

				FVector DirectionToNext = NearbyGrappleActors[i].Actor->GetActorLocation() - PlayerRef->GetActorLocation();
				DirectionToNext.Normalize();

				FVector DirectionToTarget = Target.Actor->GetActorLocation() - PlayerRef->GetActorLocation();
				DirectionToTarget.Normalize();

				if (FVector::DotProduct(CameraForward, DirectionToNext) > FVector::DotProduct(CameraForward, DirectionToTarget))
					Target = NearbyGrappleActors[i];
			}
		}

		Target.SetIconVisibility(true);
		PlayerRef->SetCanGrapple(true);
	}
	else
	{
		if (Target.Actor)
		{
			Target.SetIconVisibility(false);
			Target = FGrappleActor();
		}

		PlayerRef->SetCanGrapple(false);
	}
}
