
#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "ShortRangeWeapon.generated.h"

UCLASS()
class NAMAZU_API AShortRangeWeapon : public AWeapon
{
	GENERATED_BODY()
	
public:
	// Sets default values
	AShortRangeWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

};
