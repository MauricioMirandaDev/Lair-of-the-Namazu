
#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "KunaiPickup.generated.h"

UCLASS()
class NAMAZU_API AKunaiPickup : public APickup
{
	GENERATED_BODY()
	
public:
	// Sets default values
	AKunaiPickup();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Function used for collecting
	virtual void CollectPickup() override;
};
