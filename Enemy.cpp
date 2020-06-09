#include "Enemy.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision Component"));
	ExplosionFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Explosion FX"));
	ExplosionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Explosion Sound"));

	CollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	StaticMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ExplosionFX->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ExplosionSound->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	fBurstDelay = 0.15f;

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	TotalTime = 0.0f;
	TimeSinceLastShot = 0.0f;

	bHit = false;
	bDestroy = false;

	fDestroyTimer = 1.0f;

	ExplosionFX->Deactivate();
	ExplosionSound->Deactivate();

	ThisWorld = GetWorld();

	RandomStart = FMath::Rand();

	OnActorBeginOverlap.AddDynamic(this, &AEnemy::OnBeginOverlap);

}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TotalTime += DeltaTime;
	TimeSinceLastShot += DeltaTime;

	CurrentLocation = this->GetActorLocation();
	CurrentRotation = this->GetActorRotation();

	CurrentLocation.Y += FMath::Sin(TotalTime + RandomStart);

	this->SetActorLocation(CurrentLocation - (CurrentVelocity * DeltaTime));

	if (TimeSinceLastShot >= 1.0f && !bHit) {

		
			//if (fBurstDelay >= 0.15f) {

				FActorSpawnParameters Params = {};
				Params.Owner = this;

				GetWorld()->SpawnActor(EnemyProjectile, &CurrentLocation,
					&CurrentRotation, Params);

				fBurstDelay = 0.0f;
			//}

		TimeSinceLastShot = 0.0f;
		fBurstDelay += DeltaTime;
	} // 

	// 

	if (bDestroy)
		this->Destroy();

	if (bHit) {
	
		StaticMesh->SetVisibility(false);
		this->SetActorEnableCollision(false);
		
		ExplosionFX->Activate();
		ExplosionSound->Activate();

		fDestroyTimer -= DeltaTime;

	}

	if (fDestroyTimer <= 0.0f) {
		this->Destroy();
	}


}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void AEnemy::OnBeginOverlap(AActor * EnemyActor, AActor * OtherActor)
{

	if (OtherActor->ActorHasTag("Bounds")) {
		bDestroy = true;
	}

	if (OtherActor->ActorHasTag("Asteroid") || OtherActor->ActorHasTag("Player")) {
		bHit = true;
	}

	if (OtherActor->ActorHasTag("Projectile")) {
		
		bHit = true;

		if (ThisWorld) {
		

			if (FMath::RandRange(0, 10) > 7) {
			
				FVector Current_Location = this->GetActorLocation();
				FRotator Current_Rotation = this->GetActorRotation();
				FActorSpawnParameters Params = {};

				ThisWorld->SpawnActor(PickupCan, &Current_Location,
					&Current_Rotation, Params);
			}
		}
	}

} // 











