
#include "NamazuGameModeBase.h"
#include "Blueprint/UserWidget.h"

// Called when the game starts
void ANamazuGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	ChangeMenuWidget(StartingWidgetClass);
}

// Remove the current menu widgetand create a new one from the specified class, if provided
void ANamazuGameModeBase::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	if (NewWidgetClass)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);

		if (CurrentWidget)
			CurrentWidget->AddToViewport();
	}
}