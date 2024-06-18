
#include "GrappleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Namazu/Actors/Rope.h"
#include "Namazu/Characters/PlayerCharacter.h"

// Sets default values for this component's properties
UGrappleComponent::UGrappleComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Set default values
	CastRodAnim = nullptr; 
	TensionStrength = 100.0f; 
	AngleToAttachPoint = 0.0f; 
	Player = nullptr;
	RopeState = ERopeState::ROPE_Detached; 
}

// Called when the game starts
void UGrappleComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<APlayerCharacter>(GetOwner());
	if (Player == nullptr)
		return;
}

// Called every frame
void UGrappleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// Getter function to access the rope actor
ARope* UGrappleComponent::GetRope()
{
	return Rope;
}

// Getter function to access the state the rope is in
ERopeState UGrappleComponent::GetRopeState()
{
	return RopeState;
}

// Setter function to update the state the rope is in
void UGrappleComponent::SetRopeState(ERopeState UpdatedState)
{
	RopeState = UpdatedState;
}

// Spawn rope actor and attach it to the player's weapon
void UGrappleComponent::SpawnRope(UStaticMeshComponent* Component)
{
	if (RopeClass)
	{
		Rope = GetWorld()->SpawnActor<ARope>(RopeClass);
		Rope->AttachToComponent(Component, FAttachmentTransformRules::KeepRelativeTransform, TEXT("RopeSocket"));
		Rope->SetPlayerReference(Player); 
		Rope->SetActorTickEnabled(Player->RopeCount > 0);
	}
}

// Rotate the player to face the point of attachment and play animation
void UGrappleComponent::PreAttach()
{
	if (CastRodAnim)
	{
		FVector DirectionToAttachPoint = Rope->GetTarget().Actor->GetActorLocation() - Player->GetActorLocation();
		DirectionToAttachPoint.Normalize();

		Player->SetActorRotation(FRotator(0.0f, DirectionToAttachPoint.Rotation().Yaw, 0.0f));
		Player->PlayAnimMontage(CastRodAnim, 1.0f);

		Rope->SetActorTickEnabled(false); 
	}
}

// Attach rope to object 
void UGrappleComponent::AttachRope()
{
	RopeState = ERopeState::ROPE_Attached; 
	Rope->UpdateRopeAttached(true);
	Rope->GetTarget().SetIconVisibility(false); 
}

// Detach rope from object 
void UGrappleComponent::DetachRope()
{
	RopeState = ERopeState::ROPE_Detached;
	Rope->UpdateRopeAttached(false);
	Rope->GetTarget().Clear(); 
	Rope->SetActorTickEnabled(true);
}

// Set the player to always rotate around the attached point
void UGrappleComponent::AttachedMovement()
{
	FVector DirectionToAttachPoint = Rope->GetTarget().Actor->GetActorLocation() - Player->GetActorLocation();
	DirectionToAttachPoint.Normalize(); 

	Player->SetActorRotation(FRotator(0.0f, DirectionToAttachPoint.Rotation().Yaw, 0.0f));

	// Get the angle between the player's forward vector and the direction to the point of attachment 
	AngleToAttachPoint = UKismetMathLibrary::DegAcos(FVector::DotProduct(Player->GetActorForwardVector(), DirectionToAttachPoint) / 
						 (Player->GetActorForwardVector().Size() * DirectionToAttachPoint.Size()));

	TensionForce(); 
}

// Calculate tension force between player and point of attachment
void UGrappleComponent::TensionForce()
{
	if (Player->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling && Player->GetCombatState() != ECombatState::COMBAT_DeadFalling)
	{
		FVector DirectionToPlayer = Player->GetActorLocation() - Rope->GetTarget().Actor->GetActorLocation();
		DirectionToPlayer.Normalize(); 

		FVector Tension = DirectionToPlayer * FVector::DotProduct(Player->GetVelocity(), DirectionToPlayer);

		Player->GetCharacterMovement()->AddForce(Tension * -TensionStrength * Player->GetCharacterMovement()->Mass);
	}
}

