
#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "RopePickup.generated.h"

UCLASS()
class COMBATPRACTICE_API ARopePickup : public APickup
{
	GENERATED_BODY()
	
public:
	// Sets default values
	ARopePickup(); 

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Function used for collecting
	virtual void CollectPickup() override;
};
