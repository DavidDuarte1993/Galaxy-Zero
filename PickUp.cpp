#include "PickUp.h"
#include "SS_Player.h"

APickUp::APickUp()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	CollisionMesh = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision Component"));

	StaticMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CollisionMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

}

void APickUp::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &APickUp::OnBeginOverlap);
}

void APickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	this->SetActorLocation(FVector(this->GetActorLocation().X - (DeltaTime * 100.0f),
		this->GetActorLocation().Y, 0.0f));

}

void APickUp::OnBeginOverlap(AActor * PickupActor, AActor * OtherActor)
{
	
	if (OtherActor->ActorHasTag("Player")) {
	
		ASS_Player* PlayerActor = Cast<ASS_Player>(OtherActor);
		PlayerActor->CollectablePickup();

		this->Destroy();

	}

	if (OtherActor->ActorHasTag("Bounds"))
		this->Destroy();
}

