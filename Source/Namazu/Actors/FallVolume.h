
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FallVolume.generated.h"

UCLASS()
class NAMAZU_API AFallVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFallVolume();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function used for overlap event
	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Box component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Volume;
};
