
#include "GrapplePoint.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
AGrapplePoint::AGrapplePoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create scene component and set as root
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root); 

	// Create sphere component and set default values
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	Capsule->SetupAttachment(Root); 
	Capsule->SetCollisionProfileName(TEXT("GrapplePoint"), true);

	// Create a widget component and set default values
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Selected Icon"));
	Widget->SetupAttachment(Root);
	Widget->SetVisibility(false);
}

// Called when the game starts or when spawned
void AGrapplePoint::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGrapplePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Create struct variable from this actor's information
FGrappleActor AGrapplePoint::CreateGrappleActor()
{
	return FGrappleActor(this, Widget);
}

