#include "ProximityBoost_Component.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "GAME/PlayerCharacter/PlayerCharacter_Base.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Game/PlayerState/PlayerState_Base.h"
#include "Net/UnrealNetwork.h"


///////////////////////////////////////////////////
////  Constructor & BeginPlay & TickComponent  ////
///////////////////////////////////////////////////
UProximityBoost_Component::UProximityBoost_Component()
{
	PrimaryComponentTick.bCanEverTick = false;

	SphereRadius = 200.0f; 
	EffectDuration = 2.0f; 
	bCanInfiniteDashAndDoubleJump = false;
	bIsStillInRangeOfProximity = false;
	bIsActivatedProximity = false;
	SetIsReplicatedByDefault(true);

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
	StaticMesh->SetupAttachment(Sphere);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Disable collision
	StaticMesh->SetVisibility(true); // Ensure it's visible
	
}
void UProximityBoost_Component::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwner())
	{
		UE_LOG(LogTemp, Error, TEXT("ProximityBoost_Component: Owner is null!"));
		return;
	}

	if (Sphere)
	{
		Sphere->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}

	if (StaticMesh && Sphere)
	{
		StaticMesh->AttachToComponent(Sphere, FAttachmentTransformRules::KeepRelativeTransform);
	}

	if (Sphere)
	{
		// Bind collision events
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &UProximityBoost_Component::OnSphereOverlapBegin);
		Sphere->OnComponentEndOverlap.AddDynamic(this , &UProximityBoost_Component::OnSphereOverlapEnd);
	}

}
void UProximityBoost_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


///////////////////////////////
////  Begin & End Overlap  ////
///////////////////////////////
void UProximityBoost_Component::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (OtherActor && OtherActor != GetOwner())
	{
		if (APlayerCharacter_Base* OtherPlayer = Cast<APlayerCharacter_Base>(OtherActor))
		{
			// Check if the overlapping component is the other player's SphereComponent
			if (OtherComp == OtherPlayer->FindComponentByClass<UProximityBoost_Component>()->Sphere)
			{

				// Set is still in range active
				bIsStillInRangeOfProximity = true;
				bIsActivatedProximity = true;
				
				ActivateProximityBoost();
				
				// Changing Colors to Green for the Static Mesh
				if (StaticMesh)
				{
					UMaterialInstanceDynamic* DynamicMaterial = StaticMesh->CreateAndSetMaterialInstanceDynamic(0);
					if (DynamicMaterial)
					{
						DynamicMaterial->SetVectorParameterValue("BaseColor", FColor::Green);
					}
				}
			}
		}
	}
}
void UProximityBoost_Component::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != GetOwner())
	{
		if (APlayerCharacter_Base* OtherPlayer = Cast<APlayerCharacter_Base>(OtherActor))
		{
			// Check if the overlapping component is the other player's SphereComponent
			if (OtherComp == OtherPlayer->FindComponentByClass<UProximityBoost_Component>()->Sphere)
			{
				// Start a timer to deactivate the boost after the delay
				GetWorld()->GetTimerManager().SetTimer(ProximityEffectTimer, this, &UProximityBoost_Component::ServerDeActivateProximityBoost, EffectDuration, false);
				bIsStillInRangeOfProximity = false;
			}
		}
	}
}

///////////////////////////////
////   Replication_OnRep   ////
///////////////////////////////
void UProximityBoost_Component::OnRep_ProximityState()
{
	APlayerCharacter_Base* Character = Cast<APlayerCharacter_Base>(GetOwner());
	if (Character)
	{
		Character->EnableSuperJump(bCanInfiniteDashAndDoubleJump);
	}
}

////////////////////////////////////////////////
////  Activate & Deactivate ProximityBoost  ////
////////////////////////////////////////////////
void UProximityBoost_Component::ActivateProximityBoost()
{
	
	if (bCanInfiniteDashAndDoubleJump) return;
	bCanInfiniteDashAndDoubleJump = true;
	if (APlayerCharacter_Base* PlayerCharacter = Cast<APlayerCharacter_Base>(GetOwner()))
	{
		if (PlayerCharacter->ReturnPlayerJumpActiveBool() == true)
		{
			PlayerCharacter->GetCharacterMovement()->JumpZVelocity = 1000;
		}
		if (PlayerCharacter->ReturnPlayerDashActiveBool() == true)
		{
			PlayerCharacter->AirDashSpeed = 3000.0f;
		}
	}
	
	OnRep_ProximityState(); //For Clients
	
}
void UProximityBoost_Component::DeactivateProximityBoost()
{
	if (!bCanInfiniteDashAndDoubleJump) return;
	if (bIsStillInRangeOfProximity) return;
	bCanInfiniteDashAndDoubleJump = false;

	bIsActivatedProximity = false;
	
	// Changing Colors to Green for the Static Mesh
	if (StaticMesh)
	{
		UMaterialInstanceDynamic* DynamicMaterial = StaticMesh->CreateAndSetMaterialInstanceDynamic(0);
		if (DynamicMaterial)
		{
			DynamicMaterial->SetVectorParameterValue("BaseColor", FColor::Red);
		}
	}

	if (APlayerCharacter_Base* PlayerCharacter = Cast<APlayerCharacter_Base>(GetOwner()))
	{
		PlayerCharacter->GetCharacterMovement()->JumpZVelocity = 500;
		PlayerCharacter->AirDashSpeed = 1500.0f;
	}

	OnRep_ProximityState(); //For Clients
}
bool UProximityBoost_Component::IsProximityBoostActive() const
{
	return bCanInfiniteDashAndDoubleJump;
}

//////////////////
/// Templates ////
//////////////////
void UProximityBoost_Component::ActivateProximityOnBeginOverlap()
{
	// Set is still in range active
	bIsStillInRangeOfProximity = true;
	bIsActivatedProximity = true;
				
	ActivateProximityBoost();
				
	// Changing Colors to Green for the Static Mesh
	if (StaticMesh)
	{
		UMaterialInstanceDynamic* DynamicMaterial = StaticMesh->CreateAndSetMaterialInstanceDynamic(0);
		if (DynamicMaterial)
		{
			DynamicMaterial->SetVectorParameterValue("BaseColor", FColor::Green);
		}
	}
}
void UProximityBoost_Component::DeActivateProximityOnEndOverlap()
{
	// Start a timer to deactivate the boost after the delay
	GetWorld()->GetTimerManager().SetTimer(ProximityEffectTimer, this, &UProximityBoost_Component::DeactivateProximityBoost, EffectDuration, false);
	bIsStillInRangeOfProximity = false;
}

////////////////////////////////
/// Activate Proximity Boost ///
////////////////////////////////
void UProximityBoost_Component::ServerActivateProximityBoost_Implementation()
{
	// Set is still in range active
	bIsStillInRangeOfProximity = true;
	bIsActivatedProximity = true;
				
	ActivateProximityBoost();
	MulticastActivateProximityBoost();
	
	// Changing Colors to Green for the Static Mesh
	if (StaticMesh)
	{
		UMaterialInstanceDynamic* DynamicMaterial = StaticMesh->CreateAndSetMaterialInstanceDynamic(0);
		if (DynamicMaterial)
		{
			DynamicMaterial->SetVectorParameterValue("BaseColor", FColor::Green);
		}
	}
}
bool UProximityBoost_Component::ServerActivateProximityBoost_Validate()
{
	return true;
}
void UProximityBoost_Component::MulticastActivateProximityBoost_Implementation()
{
	// Set is still in range active
	bIsStillInRangeOfProximity = true;
	bIsActivatedProximity = true;
				
	ActivateProximityBoost();
	
	// Changing Colors to Green for the Static Mesh
	if (StaticMesh)
	{
		UMaterialInstanceDynamic* DynamicMaterial = StaticMesh->CreateAndSetMaterialInstanceDynamic(0);
		if (DynamicMaterial)
		{
			DynamicMaterial->SetVectorParameterValue("BaseColor", FColor::Green);
		}
	}
}

//////////////////////////////////
/// DeActivate Proximity Boost ///
//////////////////////////////////
void UProximityBoost_Component::ServerDeActivateProximityBoost_Implementation()
{
	if (!bCanInfiniteDashAndDoubleJump) return;
	if (bIsStillInRangeOfProximity) return;  // If player is still in range, cancel deactivation

	bCanInfiniteDashAndDoubleJump = false;
	bIsActivatedProximity = false;

	// Call multicast to update all clients
	MulticastDeActivateProximityBoost();
}
bool UProximityBoost_Component::ServerDeActivateProximityBoost_Validate()
{
	return true;
}

void UProximityBoost_Component::MulticastDeActivateProximityBoost_Implementation()
{
	bCanInfiniteDashAndDoubleJump = false;
	bIsActivatedProximity = false;

	// Change color to red when the effect ends
	if (StaticMesh)
	{
		UMaterialInstanceDynamic* DynamicMaterial = StaticMesh->CreateAndSetMaterialInstanceDynamic(0);
		if (DynamicMaterial)
		{
			DynamicMaterial->SetVectorParameterValue("BaseColor", FColor::Red);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Proximity Boost deactivated for all clients."));

	if (APlayerCharacter_Base* PlayerCharacter = Cast<APlayerCharacter_Base>(GetOwner()))
	{
		PlayerCharacter->GetCharacterMovement()->JumpZVelocity = 500;
		PlayerCharacter->AirDashSpeed = 1500.0f;
	}

	OnRep_ProximityState(); //For Clients
}

////////////////////////////////////////////////
///////////       Replication      /////////////
////////////////////////////////////////////////
void UProximityBoost_Component::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UProximityBoost_Component, bCanInfiniteDashAndDoubleJump);
	DOREPLIFETIME(UProximityBoost_Component, bIsStillInRangeOfProximity);
	DOREPLIFETIME(UProximityBoost_Component, bIsActivatedProximity);
}
