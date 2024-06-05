
#pragma once

#include "CoreMinimal.h"
#include "Namazu/Actors/GrappleActor.h"
#include "GrappleInterface.generated.h"

class UGrappleComponent;

UINTERFACE(BlueprintType)
class UGrappleInterface : public UInterface
{
	GENERATED_BODY()
};

class IGrappleInterface
{
	GENERATED_BODY()

public:
	virtual FGrappleActor CreateGrappleActor(); 
};
