
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatPractice/GrappleInterface.h"
#include "GrapplePoint.generated.h"

UCLASS()
class COMBATPRACTICE_API AGrapplePoint : public AActor, public IGrappleInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrapplePoint();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Interface function
	virtual FGrappleActor CreateGrappleActor() override; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Visual components 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* Widget;
};
