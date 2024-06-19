
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NamazuGameModeBase.generated.h"

class APlayerCharacter; 
class ACombatPlayerController;
class UNamazuSaveGame;

UCLASS()
class NAMAZU_API ANamazuGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    // Sets default values
    ANamazuGameModeBase(); 

    /** Remove the current menu widget and create a new one from the specified class, if provided. */
    UFUNCTION(BlueprintCallable, Category = "UMG Game")
    void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

    // Functions and variables used for gameplay
    UFUNCTION(BlueprintCallable)
    void PauseGame(); 

    UFUNCTION(BlueprintCallable)
    void UnpauseGame();

    UFUNCTION(BlueprintCallable)
    void GameOver();

    static APlayerCharacter* PlayerRef; 

    static ACombatPlayerController* PlayerControllerRef; 

    // Functions and variable used for save system
    UFUNCTION(BlueprintCallable)
    void SaveGame(); 

    UFUNCTION(BlueprintCallable)
    void LoadGame();

protected:
    /** Called when the game starts. */
    virtual void BeginPlay() override;

    /** The widget class we will use as our menu when the game starts. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
    TSubclassOf<UUserWidget> StartingWidgetClass;

    /** The widget instance that we are using as our menu. */
    UPROPERTY()
    UUserWidget* CurrentWidget;

    // Components used for gameplay
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menus")
    TSubclassOf<UUserWidget> PauseWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Menus")
    TSubclassOf<UUserWidget> GameOverWidgetClass;

    // Components used for save system
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Save System")
    TSubclassOf<UNamazuSaveGame> NamazuSaveGameClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Save System")
    FString SaveGameSlotName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Save System")
    int32 SaveGameSlotIndex; 

    UPROPERTY()
    UNamazuSaveGame* NamazuSaveGame;
};
