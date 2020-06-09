#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Projectile.h"
#include "PickUp.h"

#include "Enemy.generated.h"

UCLASS()
class SPACE_SHOOTER_API AEnemy : public APawn
{
	GENERATED_BODY()

public:
	AEnemy();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
		UCapsuleComponent* CollisionComponent;

	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* ExplosionFX;

	UPROPERTY(EditAnywhere)
		UAudioComponent* ExplosionSound;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AProjectile> EnemyProjectile;

	UWorld* ThisWorld;

	UPROPERTY(EditAnywhere)
		TSubclassOf<APickUp> PickupCan;

	UPROPERTY(EditAnywhere)
		FVector CurrentVelocity;

	float TotalTime;
	float TimeSinceLastShot;
	float RandomStart;

	bool bHit;
	bool bDestroy;

	float fDestroyTimer;
	float fBurstDelay;

	FVector CurrentLocation;
	FRotator CurrentRotation;

	UFUNCTION()
		void OnBeginOverlap(AActor* EnemyActor, AActor* OtherActor);


protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
