#include "ProximityBoost_Component.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "GAME/PlayerCharacter/PlayerCharacter_Base.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

///////////////////////////////////////////////////
////  Constructor & BeginPlay & TickComponent  ////
///////////////////////////////////////////////////
UProximityBoost_Component::UProximityBoost_Component()
{
	PrimaryComponentTick.bCanEverTick = false;

	SphereRadius = 300.0f; 
	EffectDuration = 2.0f; 
	bCanInfiniteDashAndDoubleJump = false;
	bIsStillInRangeOfProximity = false;
	SetIsReplicatedByDefault(true);

	// Add a sphere component to detect proximity
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	Sphere->SetSphereRadius(SphereRadius);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	Sphere->SetCollisionResponseToAllChannels(ECR_Overlap);

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
				ActivateProximityBoost();

				// Set is still in range active
				bIsStillInRangeOfProximity = true;
				
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
				GetWorld()->GetTimerManager().SetTimer(ProximityEffectTimer, this, &UProximityBoost_Component::DeactivateProximityBoost, EffectDuration, false);
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
		PlayerCharacter->GetCharacterMovement()->JumpZVelocity = 1000;
		PlayerCharacter->AirDashSpeed = 3000.0f;
	}
	OnRep_ProximityState(); //For Clients
}
void UProximityBoost_Component::DeactivateProximityBoost()
{
	if (!bCanInfiniteDashAndDoubleJump) return;
	if (bIsStillInRangeOfProximity) return;
	
	bCanInfiniteDashAndDoubleJump = false;
	
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

////////////////////////////////////////////////
///////////       Replication      /////////////
////////////////////////////////////////////////
void UProximityBoost_Component::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UProximityBoost_Component, bCanInfiniteDashAndDoubleJump);
	DOREPLIFETIME(UProximityBoost_Component, bIsStillInRangeOfProximity);
}
