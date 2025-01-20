#include "TrailActor.h"
#include "GAME/PlayerCharacter/PlayerCharacter_Base.h"
#include "GAME/ProximityBoost/ProximityBoost_Component.h"
#include "Net/UnrealNetwork.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ATrailActor::ATrailActor()
{

	PrimaryActorTick.bCanEverTick = false;

	SphereRadius = 100.0f; 

	// Add a sphere component to detect proximity
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	Sphere->SetSphereRadius(SphereRadius);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//Sphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	//Sphere->SetCollisionResponseToAllChannels(ECR_Overlap);
	Sphere->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1); // ProximityBoost channel
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // Ignore all other channels
	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap); // Overlap with other ProximityBoost components
	
	// Static Mesh Component
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Disable collision
	StaticMesh->SetVisibility(true); // Ensure it's visible
	
	
	bReplicates = true;
	
}



void ATrailActor::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (OtherActor && OtherActor != GetOwner())  // Ensure owner doesn't trigger boost
	{
		if (APlayerCharacter_Base* OtherPlayer = Cast<APlayerCharacter_Base>(OtherActor))
		{
			OtherPlayer->ActivatePlayersProximityBoost();
			UE_LOG(LogTemp, Log, TEXT("TrailerActor activated for"));
		}
	}
}

void ATrailActor::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != GetOwner()) 
	{
		if (APlayerCharacter_Base* OtherPlayer = Cast<APlayerCharacter_Base>(OtherActor))
		{
			OtherPlayer->DeActivatePlayersProximityBoost();
			UE_LOG(LogTemp, Log, TEXT("TrailerActor deactivated"));
		}
	}
}

void ATrailActor::BeginPlay()
{
	Super::BeginPlay();

	// Draw a debug sphere with a limited lifetime
	DrawDebugSphere(GetWorld(), GetActorLocation(), SphereRadius, 16, FColor::Red, false, 5.0f, 0, 2.0f);
	

	if (Sphere)
	{
		Sphere->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}

	if (StaticMesh && Sphere)
	{
		StaticMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}

	if (Sphere)
	{
		// Bind collision events
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &ATrailActor::OnSphereOverlapBegin);
		Sphere->OnComponentEndOverlap.AddDynamic(this , &ATrailActor::OnSphereOverlapEnd);
	}
}

void ATrailActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATrailActor, SphereRadius);
}

 void ATrailActor::DestroySelf()
{
	// Clear debug sphere by redrawing it with zero lifetime
	DrawDebugSphere(GetWorld(), GetActorLocation(), SphereRadius, 16, FColor::Red, false, 0.0f, 0, 2.0f);

	UE_LOG(LogTemp, Log, TEXT("TrailActor %s is being destroyed"), *GetName());
	Destroy();
}
