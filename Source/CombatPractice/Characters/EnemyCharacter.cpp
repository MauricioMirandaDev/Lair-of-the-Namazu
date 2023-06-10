
#include "EnemyCharacter.h"
#include "CombatPractice/Characters/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyCharacter"), true);
	SearchRadius = 100.0f;
	MaxSightAngle = 90.0f; 
	PlayerReference = nullptr;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Get a reference to the player
	PlayerReference = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerReference == nullptr)
		return;
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 
}

// Perform a line of sight calculation to determine if the enemy can see the player
bool AEnemyCharacter::CanSeePlayer()
{
	if (IsPlayerWithinReach())
	{
		// Calculate the angle between the enemy's forward vector and direction to player
		FVector DirectionToPlayer = PlayerReference->GetActorLocation() - GetActorLocation();
		DirectionToPlayer.Normalize();

		float AngleBetweenPlayer = UKismetMathLibrary::DegAcos(FVector::DotProduct(GetActorForwardVector(), DirectionToPlayer));

		if (AngleBetweenPlayer <= MaxSightAngle && !IsPlayerBlocked())
			return true;
	}

	return false;
}

// Perform a sphere trace to see if the player is withing detection radius
bool AEnemyCharacter::IsPlayerWithinReach()
{
	TArray<AActor*> ActorsToIngore;
	ActorsToIngore.Add(this);

	FHitResult SphereResult; 

	return UKismetSystemLibrary::SphereTraceSingle(GetWorld(), GetActorLocation(), GetActorLocation(), SearchRadius, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2),
												   true, ActorsToIngore, EDrawDebugTrace::None, OUT SphereResult, true);
}

// Perform a line trace to see if ray to player is blockd by anything
bool AEnemyCharacter::IsPlayerBlocked()
{
	TArray<AActor*> ActorsToIngore;
	ActorsToIngore.Add(this);

	FHitResult LineResult; 

	return UKismetSystemLibrary::LineTraceSingle(GetWorld(), GetActorLocation(), PlayerReference->GetActorLocation(), UEngineTypes::ConvertToTraceType(ECC_Visibility),
												 true, ActorsToIngore, EDrawDebugTrace::None, OUT LineResult, true);
}
