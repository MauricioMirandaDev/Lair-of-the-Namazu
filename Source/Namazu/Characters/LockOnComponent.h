
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

	// Friend class that access functions and variables
	friend class APlayerCharacter; 

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// Components, functions, and variables for lock-on system
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lock-On System", meta = (AllowPrivateAccess = "true"))
	float MaxLockOnDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lock-On System", meta = (AllowPrivateAccess = "true"))
	float ForwardOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lock-On System", meta = (AllowPrivateAccess = "true"))
	float UpOffset;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsPlayerLockedOn; 

	void UpdatedLockedOnEnemy(); 

	void ClearLockedOnEnemy(); 

	AEnemyCharacter* FindClosestEnemy();

	void LockedOnMovement(); 

	APlayerCharacter* Player; 

	AEnemyCharacter* LockedOnEnemy;

	AActor* TraceOrigin; 

	FVector LockOnDirection;
};
