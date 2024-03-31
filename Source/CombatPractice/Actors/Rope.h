
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rope.generated.h"

class AGrapplePoint;
class APlayerCharacter; 

UCLASS()
class COMBATPRACTICE_API ARope : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARope();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Getter functions
	AGrapplePoint* GetTargetPoint(); 

	// Setter functions
	void SetPlayerReference(APlayerCharacter* Player);

	// Function for cable
	void UpdateRopeAttached(bool bShouldAttach);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	// Physical components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCableComponent* Cable;

	// Function and variables for finding grapple points
	void DetermineClosestGrapplePoint(); 

	AGrapplePoint* TargetGrapplePoint; 

	APlayerCharacter* PlayerRef; 
};
