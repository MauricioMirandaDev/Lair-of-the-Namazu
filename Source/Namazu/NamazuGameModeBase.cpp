
#include "NamazuGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Namazu/NamazuSaveGame.h"
#include "Namazu/Characters/PlayerCharacter.h"
#include "Namazu/Controllers/CombatPlayerController.h"

APlayerCharacter* ANamazuGameModeBase::PlayerRef; 
ACombatPlayerController* ANamazuGameModeBase::PlayerControllerRef;

// Set default values
ANamazuGameModeBase::ANamazuGameModeBase()
{
	SaveGameSlotName = TEXT("Slot1");
	SaveGameSlotIndex = 0;
}

// Called when the game starts
void ANamazuGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// Create or load save file 
	if (UGameplayStatics::DoesSaveGameExist(SaveGameSlotName, SaveGameSlotIndex))
	{
		NamazuSaveGame = Cast<UNamazuSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameSlotName, SaveGameSlotIndex));

		if (NamazuSaveGame == nullptr)
			return; 
	}
	else
	{
		NamazuSaveGame = Cast<UNamazuSaveGame>(UGameplayStatics::CreateSaveGameObject(NamazuSaveGameClass));

		if (NamazuSaveGame)
		{
			UGameplayStatics::SaveGameToSlot(NamazuSaveGame, SaveGameSlotName, SaveGameSlotIndex);
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("FIRST SAVE"));
		}
	}

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

// Pause gameplay and show pause menu
void ANamazuGameModeBase::PauseGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	ChangeMenuWidget(PauseWidgetClass);
	PlayerControllerRef->SetInputMode(FInputModeGameAndUI());
	PlayerControllerRef->SetShowMouseCursor(true);
}

// Remove pause menu and resume gameplay
void ANamazuGameModeBase::UnpauseGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	ChangeMenuWidget(StartingWidgetClass);
	PlayerControllerRef->SetInputMode(FInputModeGameOnly());
	PlayerControllerRef->SetShowMouseCursor(false);
}

// Handle logic for game over 
void ANamazuGameModeBase::GameOver()
{
	ChangeMenuWidget(GameOverWidgetClass);
	PlayerControllerRef->SetInputMode(FInputModeUIOnly());
	PlayerControllerRef->SetShowMouseCursor(true);
}

// Save data to save game object
void ANamazuGameModeBase::SaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveGameSlotName, SaveGameSlotIndex))
	{
		NamazuSaveGame->PlayerTransform = PlayerRef->GetActorTransform();

		UGameplayStatics::SaveGameToSlot(NamazuSaveGame, SaveGameSlotName, SaveGameSlotIndex);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("SAVE SUCCESSFUL"));
	}
}

// Load data to save game object
void ANamazuGameModeBase::LoadGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveGameSlotName, SaveGameSlotIndex))
	{
		PlayerRef->SetActorTransform(NamazuSaveGame->PlayerTransform);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("LOAD SUCCESSFUL"));

		UnpauseGame(); 
	}
}
