
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "NamazuSaveGame.generated.h"

UCLASS()
class NAMAZU_API UNamazuSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	// Sets default values
	UNamazuSaveGame();

	UPROPERTY();
	FTransform PlayerTransform; 

protected:

private:

};
