
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockOnComponent.generated.h"

class AEnemyCharacter; 
class APlayerCharacter; 

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NAMAZU_API ULockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULockOnComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Getter function
	AEnemyCharacter* GetLockedOnEnemy(); 

	// Setter function
	void SetLockedOnEnemy(AEnemyCharacter* NewEnemy); 
	
	// Functions for lock-on system
	UFUNCTION(BlueprintPure)
	bool IsPlayerLockedOn(); 

	AEnemyCharacter* FindClosestEnemy(AActor* TraceOrigin, FVector Direction);

	void LockedOnMovement(); 

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// Components, function, and variables for lock-on system
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lock-On System", meta = (AllowPrivateAccess = "true"))
	float MaxLockOnDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lock-On System", meta = (AllowPrivateAccess = "true"))
	float ForwardOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lock-On System", meta = (AllowPrivateAccess = "true"))
	float UpOffset;

	bool IsEnemyBlocked(AActor* TraceOrigin, AActor* TraceEnd);

	APlayerCharacter* Player; 

	AEnemyCharacter* LockedOnEnemy;
};
