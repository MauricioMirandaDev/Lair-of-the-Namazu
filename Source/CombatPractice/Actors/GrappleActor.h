
#pragma once

#include "CoreMinimal.h"
#include "GrappleActor.generated.h"

class AActor;
class UWidgetComponent;

USTRUCT(BlueprintType)
struct FGrappleActor
{
	GENERATED_BODY()

public:
	FGrappleActor();

	FGrappleActor(AActor* InActor, UWidgetComponent* InIcon); 

	void SetIconVisibility(bool bNewVisibility); 

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AActor* Actor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UWidgetComponent* Icon;
};
