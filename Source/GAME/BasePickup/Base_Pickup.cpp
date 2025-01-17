#include "Base_Pickup.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "GAME/PlayerCharacter/PlayerCharacter_Base.h"
#include "TimerManager.h"


ABase_Pickup::ABase_Pickup()
{

	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>("Collision");
	RootComponent = SphereCollision;
	SphereCollision->SetGenerateOverlapEvents(true);
	SphereCollision->SetSphereRadius(200.0f);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(SphereCollision);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bReplicates = true;

	// Default Respawn Time
	RespawnTime = 10.0f;

}


void ABase_Pickup::BeginPlay()
{
	
	Super::BeginPlay();
	
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ABase_Pickup::OnBeginOverlap);

}

void ABase_Pickup::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter_Base* PlayerCharacter = Cast<APlayerCharacter_Base>(OtherActor))
	{
		Pickup(PlayerCharacter);

		// Destroy the pickup after applying the effect
       // Destroy();

		
		// Disable the pickup temporarily instead of destroying it
		SetActorEnableCollision(false);
		SetActorHiddenInGame(true);

		// Start the respawn timer
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ABase_Pickup::RespawnPickup, RespawnTime, false);
	}
}

void ABase_Pickup::Pickup_Implementation(class ACharacter* OwningCharacter)
{
	if (APlayerCharacter_Base* PlayerCharacter = Cast<APlayerCharacter_Base>(OwningCharacter))
	{
		if (PlayerCharacter->HasAuthority())
		{
			// Call the CollectPickup function on the player character
			PlayerCharacter->CollectPickup();

			
		}

		SetOwner(PlayerCharacter);
	}
	
}

void ABase_Pickup::RespawnPickup()
{
	// Re-enable the pickup
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);

	UE_LOG(LogTemp, Warning, TEXT("Pickup has respawned!"));
}


void ABase_Pickup::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);

}

