#include "Hazard.h"

AHazard::AHazard()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	AsteroidExplosionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("AsteroidExplosionSound"));
	AsteroidExplosionFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("AstreoidExplosionFX"));

	StaticMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	AsteroidExplosionSound->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	AsteroidExplosionFX->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	Spawn_X_Velocity = -500.0f;
	Spawn_Y_Velocity = 0.0f;

	SelfDestructTimer = 1.0f;

	bHit = false;
}

void AHazard::BeginPlay()
{
	Super::BeginPlay();
	
	Initial_Rotation = 0.0f;
	Initial_X_Location = 1500.0f;
	Initial_Y_Location = FMath::RandRange(-400, 400);
	
	RandSize = FMath::RandRange(3, 8);
	this->SetActorScale3D(FVector(RandSize, RandSize, RandSize));

	AsteroidExplosionSound->Deactivate();
	AsteroidExplosionSound->bStopWhenOwnerDestroyed = false;

	AsteroidExplosionFX->Deactivate();

	OnActorBeginOverlap.AddDynamic(this, &AHazard::OnBeginOverlap);


}

void AHazard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	check(this != nullptr);

	Initial_Rotation += DeltaTime;

	Initial_X_Location += DeltaTime * Spawn_X_Velocity;
	Initial_Y_Location += DeltaTime * Spawn_Y_Velocity;

	this->SetActorRotation(FRotator(Initial_Rotation * 100.0f, Initial_Rotation * 50.0f, 0.0f));
	this->SetActorLocation(FVector(Initial_X_Location, Initial_Y_Location, 0.0f));

	if (SelfDestructTimer <= 0.0f)
		this->Destroy();

	if (bHit) {
	
		bHit = false;
		bStartDestroyTimer = true;

		AsteroidExplosionFX->Activate();
		AsteroidExplosionFX->SetWorldLocation(this->GetActorLocation());
		AsteroidExplosionFX->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

		AsteroidExplosionSound->Activate();
		AsteroidExplosionSound->Play();

		this->StaticMesh->SetVisibility(false);
		this->SetActorEnableCollision(false);

		if (this->GetActorScale3D().X > 6.0f) {
			SpawnChildren(3);
		}

	}

	if (bStartDestroyTimer)
		SelfDestructTimer -= DeltaTime;

} // 

void AHazard::SpawnChildren(int32 NumChildren)
{

	FActorSpawnParameters Params = {};
	Params.Owner = this;

	for (int i = 0; i < NumChildren; i++) {
	
		if (ChildSpawn != nullptr) {
		
			AHazard* NewHazard = Cast<AHazard>(GetWorld()->SpawnActor(
				ChildSpawn, new FVector(this->GetActorLocation()),
				new FRotator(this->GetActorRotation()), Params));

			NewHazard->Initial_X_Location = this->GetActorLocation().X;
			NewHazard->Initial_Y_Location = this->GetActorLocation().Y;

			NewHazard->SetHazardVelocity(FVector(FMath::RandRange(-250, 100), 
				FMath::RandRange(-50, 50), 0.0f));

			float RandScale = FMath::RandRange(2, 5);

			NewHazard->SetActorScale3D(FVector(RandScale, RandScale, RandScale));

			NewHazard->SetActorEnableCollision(true);
			NewHazard->StaticMesh->SetVisibility(true);
		}


	} // 

} // 

void AHazard::SetHazardVelocity(FVector NewVelocity)
{

	Spawn_X_Velocity = NewVelocity.X;
	Spawn_Y_Velocity = NewVelocity.Y;

}

void AHazard::OnBeginOverlap(AActor * AsteroidActor, AActor * OtherActor)
{

	if (OtherActor->ActorHasTag("Bounds")) 
		SelfDestructTimer = 0.0f;
	
	if (OtherActor->ActorHasTag("Projectile") || OtherActor->ActorHasTag("Player")) {
	
		bHit = true;

	}


} // 


