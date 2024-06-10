
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrappleComponent.generated.h"

class APlayerCharacter;
class ARope; 

UENUM(BlueprintType)
enum class ERopeState : uint8 {
	ROPE_Detached UMETA(DisplayName = "Rope Detached"),
	ROPE_Attaching UMETA(DisplayName = "Attaching Rope"), 
	ROPE_Attached UMETA(DisplayName = "Rope Attached"),
	ROPE_Grappling UMETA(DisplayName = "Grappling"),
	ROPE_Reeling UMETA(DisplayName = "Reeling In")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NAMAZU_API UGrappleComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrappleComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Getter functions
	ARope* GetRope(); 

	ERopeState GetRopeState();

	// Setter functions
	void SetRopeState(ERopeState UpdatedState); 

	// Functions for grapple system
	void SpawnRope(class UStaticMeshComponent* Component);

	void PreAttach(); 

	void AttachRope(); 

	void DetachRope(); 

	void AttachedMovement(); 

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	// Components, function, and variable for grapple system
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rope", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ARope> RopeClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rope", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* CastRodAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rope", meta = (AllowPrivateAccess = "true"))
	float TensionStrength; 

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ARope* Rope;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ERopeState RopeState; 

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float AngleToAttachPoint; 

	void TensionForce(); 

	APlayerCharacter* Player;
};
