#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

#include "PickUp.generated.h"

UCLASS()
class SPACE_SHOOTER_API APickUp : public AActor
{
	GENERATED_BODY()
	
public:	
	APickUp();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
		UCapsuleComponent* CollisionMesh;

	UFUNCTION()
		void OnBeginOverlap(AActor* PickupActor, AActor* OtherActor);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
