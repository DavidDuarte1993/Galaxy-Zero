#include "Projectile.h"
#include "SS_Player.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	BulletComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneComponent"));
	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));

	BulletComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CapsuleCollision->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	bHit = false;
	bDestroy = false;

}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentLocation = this->GetActorLocation();

	OnActorBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);

}


void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = FVector(CurrentLocation.X + (ProjectileSpeed * DeltaTime), 
		CurrentLocation.Y, CurrentLocation.Z);

	this->SetActorLocation(NewLocation);
	CurrentLocation = NewLocation;

	if (bHit)
		this->Destroy();

}


void AProjectile::OnBeginOverlap(AActor * ProjectileActor, AActor * OtherActor)
{
	if (OtherActor) {
	

		ASS_Player* OwningPlayer = Cast<ASS_Player>(this->GetOwner());

		if (OtherActor->ActorHasTag(FName("Bounds")))
			bHit = true;

		if (this->ActorHasTag(FName("EnemyProjectile")) && OtherActor->ActorHasTag(FName("Player")))
			bHit = true;

		if (OtherActor->ActorHasTag(FName("EnemyShip"))) {
		
			if (OwningPlayer) {

				OwningPlayer->PlayerScore += 50.0f;

				bHit = true;
			}
		}

		if (OtherActor->ActorHasTag(FName("Asteroid"))) {

			if (OwningPlayer)
				OwningPlayer->PlayerScore += 10.0f;

			bHit = true;
		}

	}

}









