#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "Hazard.generated.h"

UCLASS()
class SPACE_SHOOTER_API AHazard : public AActor
{
	GENERATED_BODY()
	
public:	
	AHazard();

	void SpawnChildren(int32 NumChildren);
	void SetHazardVelocity(FVector NewVelocity);

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
		UCapsuleComponent* CollisionComponent;

	UPROPERTY(EditAnywhere)
		UAudioComponent* AsteroidExplosionSound;

	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* AsteroidExplosionFX;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AHazard> ChildSpawn;

	UFUNCTION()
	void OnBeginOverlap(AActor* AsteroidActor, AActor* OtherActor);

	float Initial_Rotation;
	float Initial_X_Location;
	float Initial_Y_Location;
	float RandSize;

	float SelfDestructTimer;

	UPROPERTY(EditAnywhere)
		float Spawn_X_Velocity;

	float Spawn_Y_Velocity;

	bool bHit;
	bool bStartDestroyTimer;


protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
