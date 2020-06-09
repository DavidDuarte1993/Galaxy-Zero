#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/CapsuleComponent.h"

#include "Projectile.generated.h"

UCLASS()
class SPACE_SHOOTER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* BulletComponent;

	UPROPERTY(EditAnywhere)
		UCapsuleComponent* CapsuleCollision;

	UPROPERTY(EditAnywhere)
		float ProjectileSpeed;

	UFUNCTION()
		void OnBeginOverlap(AActor* ProjectileActor, AActor* OtherActor);

	FVector CurrentLocation;

	bool bHit;
	bool bDestroy;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
