
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class ACombatCharacter;
class UStaticMeshComponent; 

UCLASS()
class NAMAZU_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Getter function
	UStaticMeshComponent* GetMesh();

	// Setter function
	void SetOwningCharacter(ACombatCharacter* NewOwner);

	// Functions used for hitbox
	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void UpdateHitbox(bool bActivate, FVector NewBoxExtent);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Component used for hitbox
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Hitbox;

	// Reference to the character who owns this weapon
	ACombatCharacter* OwningCharacter;

private:
	// Visual components 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh; 

	// Component used for combat
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USoundBase* ImpactSoundEffect;

	ACombatCharacter* LastCharacterDamaged; 
};
