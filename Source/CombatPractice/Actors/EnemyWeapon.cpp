
#include "EnemyWeapon.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CombatPractice/AI/EnemyAIController.h"
#include "CombatPractice/Characters/CombatCharacter.h"
#include "Components/BoxComponent.h"

AEnemyWeapon::AEnemyWeapon()
{
}

void AEnemyWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyWeapon::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::BeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (AEnemyAIController* AIController = Cast<AEnemyAIController>(OwningCharacter->GetController()))
		AIController->GetBlackboardComponent()->SetValueAsBool(TEXT("HitSuccessful"), true);
}

void AEnemyWeapon::UpdateHitbox(bool bActivate)
{
	Super::UpdateHitbox(bActivate);

	if (bActivate)
		Hitbox->SetCollisionProfileName(TEXT("EnemyWeapon"), true);
}


