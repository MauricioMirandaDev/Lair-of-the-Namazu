
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NativeGameplayTags.h"
#include "NamazuGameModeBase.generated.h"

class APickup;

UCLASS()
class NAMAZU_API ANamazuGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    /** Remove the current menu widget and create a new one from the specified class, if provided. */
    UFUNCTION(BlueprintCallable, Category = "UMG Game")
    void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

    // Blueprint event that pauses game
    UFUNCTION(BlueprintImplementableEvent)
    void PauseGame(); 

    // Blueprint event that implements game over logic 
    UFUNCTION(BlueprintImplementableEvent)
    void GameOver();

protected:
    /** Called when the game starts. */
    virtual void BeginPlay() override;

    /** The widget class we will use as our menu when the game starts. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
    TSubclassOf<UUserWidget> StartingWidgetClass;

    /** The widget instance that we are using as our menu. */
    UPROPERTY()
    UUserWidget* CurrentWidget;
};
