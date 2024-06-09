
#include "LockOnComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Namazu/Characters/EnemyCharacter.h"
#include "Namazu/Characters/PlayerCharacter.h"

// Sets default values for this component's properties
ULockOnComponent::ULockOnComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Set default values
	MaxLockOnDistance = 500.0f;
	ForwardOffset = 500.0f;
	UpOffset = 500.0f;  
	LockedOnEnemy = nullptr; 
}

// Called when the game starts
void ULockOnComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<APlayerCharacter>(GetOwner());
	if (Player == nullptr)
		return; 
}

// Called every frame
void ULockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// Getter function to access the locked on enemy
AEnemyCharacter* ULockOnComponent::GetLockedOnEnemy()
{
	return LockedOnEnemy;
}

// Set the new enemy the player is locked onto
void ULockOnComponent::SetLockedOnEnemy(AEnemyCharacter* NewEnemy)
{
	// Turn off the original enemy's lock on target
	if (IsPlayerLockedOn())
		LockedOnEnemy->GetLockOnTarget()->SetVisibility(false); 

	LockedOnEnemy = NewEnemy; 

	// If the new enemy is not null, turn on the new enemy's lock on target
	if (IsPlayerLockedOn())
		LockedOnEnemy->GetLockOnTarget()->SetVisibility(true); 
}

// Return whether the player is locked onto an enemy or not
bool ULockOnComponent::IsPlayerLockedOn()
{
	return LockedOnEnemy != nullptr;
}

// Find the enemy closest to the player
AEnemyCharacter* ULockOnComponent::FindClosestEnemy(AActor* TraceOrigin, FVector Direction)
{
	// Variables for tracing
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(TraceOrigin);

	TArray<AActor*> FoundActors;

	AActor* ClosestEnemy;

	FHitResult HitResult; 

	// Perform sphere overlap to get all nearby enemies
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), TraceOrigin->GetActorLocation(), MaxLockOnDistance, ObjectTypes, AEnemyCharacter::StaticClass(), ActorsToIgnore, OUT FoundActors);

	// Perform line trace to see if line of sight to enemy is blocked
	if (!FoundActors.IsEmpty())
	{
		if (!UKismetSystemLibrary::LineTraceSingle(GetWorld(), Player->GetActorLocation(), FoundActors[0]->GetActorLocation(), UEngineTypes::ConvertToTraceType(ECC_Visibility),
			false, ActorsToIgnore, EDrawDebugTrace::None, OUT HitResult, true))
		{
			ClosestEnemy = FoundActors[0];

			// If there are multiple enemies see which enemy is closer to the desired direction
			for (AActor* NextActor : FoundActors)
			{
				FVector DirectionToClosest = ClosestEnemy->GetActorLocation() - TraceOrigin->GetActorLocation();
				DirectionToClosest.Normalize();

				FVector DirectionToNext = NextActor->GetActorLocation() - TraceOrigin->GetActorLocation();
				DirectionToNext.Normalize();

				if (!UKismetSystemLibrary::LineTraceSingle(GetWorld(), Player->GetActorLocation(), NextActor->GetActorLocation(), UEngineTypes::ConvertToTraceType(ECC_Visibility),
					false, ActorsToIgnore, EDrawDebugTrace::None, OUT HitResult, true)
					&&
					FVector::DotProduct(Direction, DirectionToNext) > FVector::DotProduct(Direction, DirectionToClosest))
				{
					ClosestEnemy = NextActor;
				}
			}

			return Cast<AEnemyCharacter>(ClosestEnemy);
		}
		else
			return nullptr;

	}
	else
		return nullptr;
}


// Move the player around the enemy they are locked onto
void ULockOnComponent::LockedOnMovement()
{
	FVector DirectionToEnemy = LockedOnEnemy->GetActorLocation() - Player->GetActorLocation();

	DirectionToEnemy.Normalize();
	Player->SetActorRotation(FRotator(0.0f, DirectionToEnemy.Rotation().Yaw, 0.0f));

	FVector CameraToEnemy = LockedOnEnemy->GetActorLocation() - 
							((Player->GetActorLocation() + (Player->GetActorForwardVector() * -ForwardOffset))) - FVector(0.0f, 0.0f, UpOffset);

	CameraToEnemy.Normalize();
	Player->GetController()->SetControlRotation(CameraToEnemy.Rotation());
}
