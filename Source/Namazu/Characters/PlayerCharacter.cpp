
#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Namazu/NamazuGameModeBase.h"
#include "Namazu/Actors/GrappleActor.h"
#include "Namazu/Actors/Pickup.h"
#include "Namazu/Actors/Rope.h"
#include "Namazu/Actors/Weapon.h"
#include "Namazu/Characters/EnemyCharacter.h"
#include "Namazu/Characters/GrappleComponent.h"
#include "Namazu/Characters/LockOnComponent.h"

TMap<FGameplayTag, APickup*> APlayerCharacter::CollectedPickups; 

int32 APlayerCharacter::MaxKunai = 1;
int32 APlayerCharacter::KunaiCount = 0;
int32 APlayerCharacter::MaxMedicine = 10;
int32 APlayerCharacter::MedicineCount = 0;
int32 APlayerCharacter::MaxRope = 3;
int32 APlayerCharacter::RopeCount = 0;

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerCharacter"), true);
	HealingAmount = 10.0f; 
	PlayerFocus = EPlayerFocus::FOCUS_None; 
	bJumpPressed = false; 
	bEnemyJustDefeated = false; 
	PreviousEnemyLaunched = nullptr; 
	InitialPosition = FVector(0.0f);
	EndPosition = FVector(0.0f);

	// Create spring arm component and set default values
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	// Create camera component and set default values
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// Create lock-on component
	LockOnComponent = CreateDefaultSubobject<ULockOnComponent>(TEXT("Lock-On Component"));

	// Create grapple component
	GrappleComponent = CreateDefaultSubobject<UGrappleComponent>(TEXT("Grapple Component")); 
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	ANamazuGameModeBase::PlayerRef = this; 

	GrappleComponent->SpawnRope(Weapon->GetMesh()); 

	SetActorTickEnabled(false);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SelectFocusBehavior();
}

// Communicate to the UI the max amount of medicine the player can carry
const int32 APlayerCharacter::GetMaxMedicine()
{
	return MaxMedicine;
}

// Communicate to the UI the amount of medicine held 
const int32 APlayerCharacter::GetMedicineCount()
{
	return MedicineCount;
}

// Communicate to the UI the max amount of kunai knives the player can carry
const int32 APlayerCharacter::GetMaxKunai()
{
	return MaxKunai;
}

// Communicate to the UI the amount of kunai held 
const int32 APlayerCharacter::GetKunaiCount()
{
	return KunaiCount;
}

// Communicate to the UI the max amount of rope the player can carry
const int32 APlayerCharacter::GetMaxRope()
{
	return MaxRope;
}

// Communicate to the UI the amount of rope held
const int32 APlayerCharacter::GetRopeCount()
{
	return RopeCount; 
}

// Getter function to access lock-on component
ULockOnComponent* APlayerCharacter::GetLockOnComponent()
{
	return LockOnComponent;
}

// Getter function to access grapple component 
UGrappleComponent* APlayerCharacter::GetGrappleComponent()
{
	return GrappleComponent;
}

// Set this character to be able to move or not 
void APlayerCharacter::StopMovement(bool bPauseMovement)
{
	Super::StopMovement(bPauseMovement);

	if (bPauseMovement)
		UpdateMovementSpeed(0.0f);
	else
		UpdateMovementSpeed(RunSpeed);
}

// Used to reset variables a character uses during combat
void APlayerCharacter::ResetAttack()
{
	Super::ResetAttack();
}

// Remove the designated enemy from NearbyEnemies if it is present in the array; stop locking onto the enemy if it was previously 
void APlayerCharacter::EnemyDefeated(AActor* Enemy)
{
	if (LockOnComponent->GetLockedOnEnemy() == Enemy)
	{
		LockOnComponent->SetLockedOnEnemy(nullptr);
		bEnemyJustDefeated = true;
		PlayerFocus = EPlayerFocus::FOCUS_None;
	}
}

// Deduct damage from health and update gameplay as needed
void APlayerCharacter::TakeDamage(FAttackAnimation AttackAnimation, FVector AttackLocation)
{
	Super::TakeDamage(AttackAnimation, AttackLocation); 

	StopLockingOn(); 
}

// Apply effects after the character has finished their death animation 
void APlayerCharacter::AfterDeath()
{
	Super::AfterDeath();

	if (ANamazuGameModeBase* GameMode = Cast<ANamazuGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
		GameMode->GameOver();
}


// Attach rope during animation; enables Tick()
void APlayerCharacter::Attach()
{
	SetActorTickEnabled(true);

	GrappleComponent->AttachRope();

	if (PlayerFocus != EPlayerFocus::FOCUS_Enemy)
		PlayerFocus = EPlayerFocus::FOCUS_Rope;
}

// Called whenever the player performs a jump 
void APlayerCharacter::Jump()
{
	Super::Jump(); 

	bJumpPressed = true;
}

// Called when the player's movement mode changes
void APlayerCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode); 

	if (PrevMovementMode == EMovementMode::MOVE_Falling)
		bJumpPressed = false;
}

// Called when character runs out of health
void APlayerCharacter::OnDeath()
{
	Super::OnDeath(); 

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	SpringArm->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	PlayerFocus = EPlayerFocus::FOCUS_None;
	SetActorTickEnabled(false);

	StopLockingOn();
	LockOnComponent->DestroyComponent();

	GrappleComponent->DetachRope();
	GrappleComponent->GetRope()->Destroy();
	GrappleComponent->DestroyComponent();
}

// Called when character enters fall to death volume
void APlayerCharacter::OnFallDeath()
{
	Super::OnFallDeath();

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	SpringArm->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	PlayerFocus = EPlayerFocus::FOCUS_None;
	SetActorTickEnabled(false);

	StopLockingOn();
	LockOnComponent->DestroyComponent();

	GrappleComponent->DetachRope();
	GrappleComponent->GetRope()->Destroy();
	GrappleComponent->DestroyComponent();

	AfterDeath();
}

// Perform animation when player inputs light attack
void APlayerCharacter::LightAttackPressed()
{
	if (GetCharacterMovement()->IsFalling())
	{
		PlayAttackAnim(JumpAttack);
	}
	else
	{
		int32 SelectedVariation = FMath::RandRange(0, LightAttackVariations.Num() - 1);

		switch (SelectedVariation)
		{
		case 0:
			LightAttack.Animation = LightAttackVariations[0];
			break;
		case 1:
			LightAttack.Animation = LightAttackVariations[1];
			break;
		default:
			LightAttack.Animation = LightAttackVariations[LightAttackVariations.Num() - 1];
			break;
		}

		PlayAttackAnim(LightAttack);
	}
}

// Perform animation when player inputs heavy attack
void APlayerCharacter::HeavyAttackPressed()
{
	if (!GetCharacterMovement()->IsFalling())
		PlayAttackAnim(HeavyAttack);
}

// Perform an instant attack on an unalarmed enemy or on a tripped enemy
void APlayerCharacter::InstantAttackPressed()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));

	TArray<AActor*> ActorsToIngore;
	ActorsToIngore.Add(this);

	FHitResult HitResult;

	UKismetSystemLibrary::CapsuleTraceSingleForObjects(GetWorld(), GetActorLocation(), GetActorLocation() + (GetActorForwardVector() * 100.0f),
														GetCapsuleComponent()->GetUnscaledCapsuleRadius(), GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight(), ObjectTypes, 
														false, ActorsToIngore, EDrawDebugTrace::None, OUT HitResult, true);

	if (AEnemyCharacter* HitEnemy = Cast<AEnemyCharacter>(HitResult.GetActor()))
	{
		if (FVector::DotProduct(HitEnemy->GetActorForwardVector(), GetActorForwardVector()) > 0.9)
			ConsumeKunai(InstantAttack_Behind, HitEnemy);
		else if (HitEnemy->GetCombatState() == ECombatState::COMBAT_DamagedTrip)
			ConsumeKunai(InstantAttack_Ground, HitEnemy);
	}
}

// Heal damage by consuming medicine
void APlayerCharacter::ConsumeMedicine()
{
	if (MedicineCount > 0)
	{
		CurrentHealth += HealingAmount;
		CurrentHealth = UKismetMathLibrary::Clamp(CurrentHealth, 0.0f, MaxHealth);
		MedicineCount--; 
	}
}

// Inflict instant attack on enemy
void APlayerCharacter::ConsumeKunai(FAttackAnimation Animation, AEnemyCharacter* Enemy)
{
	if (KunaiCount > 0)
	{
		PlayAttackAnim(Animation);
		Enemy->TakeDamage(Animation, GetActorLocation());
		KunaiCount--;
		StopLockingOn(); 
	}
}

// Moves player based on the action they are currently performing
void APlayerCharacter::SelectFocusBehavior()
{
	// Select movement style based on what the player is focussing on
	switch (PlayerFocus)
	{
	case EPlayerFocus::FOCUS_Enemy:
		LockOnComponent->LockedOnMovement(); 
		break;
	case EPlayerFocus::FOCUS_Rope:
		GrappleComponent->AttachedMovement(); 
		break;
	default:
		if (bEnemyJustDefeated)
		{
			BeginLockingOn();
			bEnemyJustDefeated = false;
		}
		else
			StopLockingOn();
		break;
	}
}

// Lock onto a nearby enemy or stop locking on
void APlayerCharacter::LockOn()
{
	if (!LockOnComponent->IsPlayerLockedOn())
		BeginLockingOn();
	else
		StopLockingOn();
}

// Begin locking onto an enemy
void APlayerCharacter::BeginLockingOn()
{
	FVector CameraForward = UKismetMathLibrary::GetForwardVector(Camera->GetComponentRotation());

	LockOnComponent->SetLockedOnEnemy(LockOnComponent->FindClosestEnemy(this, CameraForward));

	if (LockOnComponent->IsPlayerLockedOn())
	{
		PlayerFocus = EPlayerFocus::FOCUS_Enemy;
		SetActorTickEnabled(true);

		GrappleComponent->GetRope()->SetActorTickEnabled(false);

		if (GrappleComponent->GetRopeState() != ERopeState::ROPE_Attached)
		{
			GrappleComponent->GetRope()->GetTarget().Clear();
			GrappleComponent->GetRope()->SetTarget(LockOnComponent->GetLockedOnEnemy()->CreateGrappleActor());
		}
	}
}

// Stop locking onto an enemy
void APlayerCharacter::StopLockingOn()
{
	if (GrappleComponent->GetRopeState() == ERopeState::ROPE_Attached)
		PlayerFocus = EPlayerFocus::FOCUS_Rope;
	else
	{
		PlayerFocus = EPlayerFocus::FOCUS_None;
		SetActorTickEnabled(false);

		GrappleComponent->GetRope()->GetTarget().Clear();
		GrappleComponent->GetRope()->SetActorTickEnabled(true);
	}

	LockOnComponent->SetLockedOnEnemy(nullptr);
}

// Switch which enemy the player is locking onto based on the desired direction 
void APlayerCharacter::SwitchLockedOnEnemy(FVector Direction)
{
	if (LockOnComponent->IsPlayerLockedOn())
	{
		LockOnComponent->SetLockedOnEnemy(LockOnComponent->FindClosestEnemy(LockOnComponent->GetLockedOnEnemy(), Direction));

		if (GrappleComponent->GetRopeState() != ERopeState::ROPE_Attached)
		{
			GrappleComponent->GetRope()->GetTarget().Clear();
			GrappleComponent->GetRope()->SetTarget(LockOnComponent->GetLockedOnEnemy()->CreateGrappleActor());
		}
	}
}

// Prepare variables for grappling (this function is called in blueprints) 
void APlayerCharacter::PrepareGrapple()
{
	GrappleComponent->SetRopeState(ERopeState::ROPE_Grappling);

	InitialPosition = GetActorLocation();
	EndPosition = GrappleComponent->GetRope()->GetTarget().Actor->GetActorLocation();

	FVector DirectionToTarget = EndPosition - InitialPosition;
	DirectionToTarget.Normalize();

	FRotator RotationToPoint = DirectionToTarget.Rotation();
	SetActorRotation(FRotator(0.0f, RotationToPoint.Yaw, 0.0f));
}

// Reset values after grappling (this function is called in blueprints) ; disables Tick() 
void APlayerCharacter::ResetRope()
{
	GrappleComponent->DetachRope(); 

	if (PlayerFocus != EPlayerFocus::FOCUS_Enemy)
		PlayerFocus = EPlayerFocus::FOCUS_None;
	
	if (LockOnComponent->IsPlayerLockedOn())
		GrappleComponent->GetRope()->SetTarget(LockOnComponent->GetLockedOnEnemy()->CreateGrappleActor());
	else
	{
		SetActorTickEnabled(false);

		GrappleComponent->GetRope()->SetActorTickEnabled(true);
	}
}

// Use linear interpolation to grapple the player to a point (this function is called in blueprints) 
void APlayerCharacter::LerpPlayerPosition(float Alpha)
{
	FVector LerpedLocation = FMath::Lerp(InitialPosition, EndPosition, Alpha);

	SetActorLocation(LerpedLocation);
}

// Launch an enemy while the player is grappling (this function is called in blueprints) 
void APlayerCharacter::LaunchEnemy(AEnemyCharacter* DamagedEnemy)
{
	if (DamagedEnemy != PreviousEnemyLaunched)
	{
		PreviousEnemyLaunched = DamagedEnemy;
		DamagedEnemy->TakeDamage(LaunchAttack, GetActorLocation());
	}
}

// Use rope resource (this function is called in blueprints) 
void APlayerCharacter::ConsumeRope()
{
	RopeCount--; 
	if (RopeCount == 0)
		GrappleComponent->GetRope()->SetActorTickEnabled(false); 
}

// Play animation to cast or detach rope
void APlayerCharacter::CastRod()
{
	switch (GrappleComponent->GetRopeState())
	{
	case ERopeState::ROPE_Detached:
		if (RopeCount > 0 && GrappleComponent->GetRope()->GetTarget().Actor)
			GrappleComponent->PreAttach();
		break;
	case ERopeState::ROPE_Attached:
		ResetRope();
		break;
	default:
		break;
	}
}

// Reel in whatever the rope is attached to
void APlayerCharacter::ReelIn()
{
	if (GrappleComponent->GetRope()->GetTarget().Actor->IsA(AEnemyCharacter::StaticClass()))
	{
		AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(GrappleComponent->GetRope()->GetTarget().Actor);

		if (Enemy->GetCombatState() != ECombatState::COMBAT_DamagedTrip)
		{
			PlayAttackAnim(TripAttack);
			Enemy->TakeDamage(TripAttack, GetActorLocation());
			RopeCount--; 
			ResetRope();
		}
	}
}

