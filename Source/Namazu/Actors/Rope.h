
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Namazu/Actors/GrappleActor.h"
#include "Rope.generated.h"

class APlayerCharacter; 

UCLASS()
class NAMAZU_API ARope : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARope();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Getter functions
	FGrappleActor GetTarget(); 

	// Setter functions
	void SetPlayerReference(APlayerCharacter* Player);

	void SetTarget(FGrappleActor NewTarget); 

	// Function for cable
	void UpdateRopeAttached(bool bShouldAttach);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	// Components for cable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCableComponent* Cable;

	// Function and variables for grapple system 
	void DetermineTarget(); 

	APlayerCharacter* PlayerRef; 

	FGrappleActor Target; 
};
