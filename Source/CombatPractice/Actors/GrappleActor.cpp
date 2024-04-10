
#include "GrappleActor.h"
#include "Components/WidgetComponent.h"

FGrappleActor::FGrappleActor()
{
	Actor = nullptr;
	Icon = nullptr; 
}

FGrappleActor::FGrappleActor(AActor* InActor, UWidgetComponent* InIcon)
{
	Actor = InActor;
	Icon = InIcon; 
}

void FGrappleActor::SetIconVisibility(bool bNewVisibility)
{
	Icon->SetVisibility(bNewVisibility); 
}