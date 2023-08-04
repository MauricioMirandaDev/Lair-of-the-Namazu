
#include "Weapon.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CombatPractice/AI/EnemyAIController.h"
#include "CombatPractice/Characters/CombatCharacter.h"
#include "CombatPractice/Characters/PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ImpactSoundEffect = nullptr;  

	// Create scene component and set as root
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// Create static mesh component and set default values
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Create box component and set default values
	Hitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Hitbox"));
	Hitbox->SetupAttachment(Mesh);
	Hitbox->SetCollisionProfileName(TEXT("NoCollision"), true);
	Hitbox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::BeginOverlap);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Apply damage to the overlapped actor 
void AWeapon::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACombatCharacter* OtherCharacter = Cast<ACombatCharacter>(OtherActor))
	{
		OtherCharacter->TakeDamage(OwningCharacter->CurrentAttackAnimation, OwningCharacter->GetActorLocation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSoundEffect, GetActorLocation());
		UpdateHitbox(false); 

		if (AEnemyAIController* AIController = Cast<AEnemyAIController>(OwningCharacter->GetController()))
			AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("HitSuccessful"), true);
	}
}

// Set hitbox to active or not
void AWeapon::UpdateHitbox(bool bActivate)
{
	if (bActivate && OwningCharacter->IsA(APlayerCharacter::StaticClass()))
		Hitbox->SetCollisionProfileName(TEXT("PlayerWeapon"));
	else if (bActivate)
		Hitbox->SetCollisionProfileName(TEXT("EnemyWeapon"));
	else
		Hitbox->SetCollisionProfileName(TEXT("NoCollision"));
}



