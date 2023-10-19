
#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "PlayerWeapon.generated.h"

UCLASS()
class COMBATPRACTICE_API APlayerWeapon : public AWeapon
{
	GENERATED_BODY()
	
public:
	// Sets default values
	APlayerWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Functions used for hitbox
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void UpdateHitbox(bool bActivate) override; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
