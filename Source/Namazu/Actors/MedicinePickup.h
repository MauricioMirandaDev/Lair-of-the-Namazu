
#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "MedicinePickup.generated.h"

UCLASS()
class NAMAZU_API AMedicinePickup : public APickup
{
	GENERATED_BODY()
	
public:
	// Sets default values
	AMedicinePickup();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function used for collider
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
