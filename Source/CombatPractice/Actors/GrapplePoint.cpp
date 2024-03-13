
#include "GrapplePoint.h"
#include "Components/SphereComponent.h"
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
	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->SetupAttachment(Root); 
	Collider->SetCollisionProfileName(TEXT("GrappleArea"), true);

	// Create a widget component and set default values
	SelectedIcon = CreateDefaultSubobject<UWidgetComponent>(TEXT("Selected Icon"));
	SelectedIcon->SetupAttachment(Root);
	SelectedIcon->SetVisibility(false); 
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

// Set the visibility of this grapple point's icon
void AGrapplePoint::SetIconVisibility(bool bShowIcon)
{
	SelectedIcon->SetVisibility(bShowIcon); 
}

