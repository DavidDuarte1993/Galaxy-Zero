#include "SS_Player.h"
#include "SS_GameMode.h"
#include "Kismet/GameplayStatics.h"

ASS_Player::ASS_Player()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ParticleSystems = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	ExplosionFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionFX"));
	DeathExplosionSound = CreateDefaultSubobject<UAudioComponent>(TEXT("DeathExplosionSound"));

	CollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ShipMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ParticleSystems->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	DeathExplosionSound->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ExplosionFX->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	MaxVelocity = 250.0f;
	Current_X_Velocity = 0.0f;
	Current_Y_Velocity = 0.0f;

	bIsFiring = false;
	WeaponFireRate = 0.25f;
	TimeSinceLastShot = 0.0f;

	PlayerScore = 0.0f;

}

void ASS_Player::BeginPlay()
{
	Super::BeginPlay();
	
	Current_Location = this->GetActorLocation();
	Current_Rotation = this->GetActorRotation();

	bHit = false;
	bDead = false;

	ExplosionFX->Deactivate();
	DeathExplosionSound->Deactivate();

	Max_Health = 100.0f;
	Current_Health = 100.0f;

	Max_Armor = 100.0f;
	Current_Armor = 100.0f;

	OnActorBeginOverlap.AddDynamic(this, &ASS_Player::OnBeginOverlap);

}

void ASS_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Current_X_Velocity != 0.0f || Current_Y_Velocity != 0.0f) {
	
		New_Location = FVector(Current_Location.X + (Current_X_Velocity * DeltaTime),
			Current_Location.Y + (Current_Y_Velocity * DeltaTime), 0);

		this->SetActorLocation(New_Location);

		Current_Location = New_Location;
	}

	if (Current_Y_Velocity > 0.1f) {
	
		this->SetActorRotation(FRotator(Current_Rotation.Pitch + 45.0f, Current_Rotation.Yaw, 
			Current_Rotation.Roll));
	}
	else if (Current_Y_Velocity < -0.1f) {
	
		this->SetActorRotation(FRotator(Current_Rotation.Pitch - 45.0f, Current_Rotation.Yaw,
			Current_Rotation.Roll));
	}
	else {
	
		this->SetActorRotation(FRotator(Current_Rotation));
	}

	if (this->GetActorLocation().X > Field_Width) 
		Current_Location = FVector(Field_Width - 1, Current_Location.Y, Current_Location.Z);

	if(this->GetActorLocation().X < -Field_Width)
		Current_Location = FVector(-Field_Width + 1, Current_Location.Y, Current_Location.Z);

	if(this->GetActorLocation().Y > Field_Height)
		Current_Location = FVector(Current_Location.X, Field_Height - 1, Current_Location.Z);

	if (this->GetActorLocation().Y < -Field_Height)
		Current_Location = FVector(Current_Location.X, -Field_Height + 1, Current_Location.Z);

	// handle firing
	if (bIsFiring) {

		if (TimeSinceLastShot > WeaponFireRate) {
		
			FireWeapon();
			TimeSinceLastShot = 0.0f;
		}

	}

	TimeSinceLastShot += DeltaTime;
	
	// 
	if (bHit) {
	
		bDead = true;

		this->ShipMesh->SetVisibility(false);
		this->ParticleSystems->SetVisibility(false);
		this->ExplosionFX->Activate();
		this->DeathExplosionSound->Activate();
		this->DeathExplosionSound->Play();

		this->SetActorTickEnabled(false);

		ASS_GameMode* GameModeREF = Cast<ASS_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		GameModeREF->bPlayerDead = true;
	}


} // 

void ASS_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ASS_Player::MoveRight);
	PlayerInputComponent->BindAxis(FName("MoveUp"), this, &ASS_Player::MoveUp);

	PlayerInputComponent->BindAction(FName("Fire"), IE_Pressed, this, &ASS_Player::StartFiring);
	PlayerInputComponent->BindAction(FName("Fire"), IE_Released, this, &ASS_Player::StopFiring);

}

void ASS_Player::MoveRight(float AxisValue)
{
	Current_X_Velocity = MaxVelocity * AxisValue;
}

void ASS_Player::MoveUp(float AxisValue)
{
	Current_Y_Velocity = MaxVelocity * AxisValue;
}

void ASS_Player::StartFiring()
{
	bIsFiring = true;
}

void ASS_Player::StopFiring()
{
	bIsFiring = false;
}

void ASS_Player::FireWeapon()
{

	FActorSpawnParameters Params = {};
	Params.Owner = this;

	AActor* SpawnedProjectile = GetWorld()->SpawnActor(WeaponProjectile_BP,
		&Current_Location, &Current_Rotation, Params);

}

void ASS_Player::CollectablePickup()
{

	if (Current_Armor < 100.0f && Current_Health == 100.0f) {
		
		Current_Armor += 10.0f;

		if(Current_Armor > 100.0f)
			Current_Armor = 100.0f;

	}
	else if (Current_Health < 100.0f) {
	
		Current_Health += 10.0f;

		if (Current_Health > 100.0f)
			Current_Health = 100.0f;
			
	}

}

void ASS_Player::OnBeginOverlap(AActor * PlayerActor, AActor * OtherActor)
{

	if (OtherActor->ActorHasTag("Asteroid") || OtherActor->ActorHasTag("EnemyShip")) {
	
		if (Current_Armor > 0.0f) {
			
			Current_Armor -= 10.0f;

			if (Current_Armor < 0.0f)
				Current_Armor = 0.0f;


		} else if (Current_Health > 0.0f) {

			Current_Health -= 10.0f;

			if (Current_Health <= 0.0f) {
			
				Current_Health = 0.0f;

				bHit = true;

			}
			
		}

	}

	if (OtherActor->ActorHasTag("EnemyProjectile")) {

		if (Current_Armor > 0.0f) {

			Current_Armor -= 25.0f;

			if (Current_Armor < 0.0f)
				Current_Armor = 0.0f;


		}
		else if (Current_Health > 0.0f) {

			Current_Health -= 25.0f;

			if (Current_Health <= 0.0f) {

				Current_Health = 0.0f;

				bHit = true;
			}
		}
	}
}


