
#pragma once

#include "CoreMinimal.h"
#include "EnemyWeapon.h"
#include "LongRangeWeapon.generated.h"

class AProjectile;

UCLASS()
class NAMAZU_API ALongRangeWeapon : public AEnemyWeapon
{
	GENERATED_BODY()
	
public:
	// Sets default values
	ALongRangeWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FireWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Class", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectile> ProjectileClass;
};
