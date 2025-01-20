#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "TrailActor.generated.h"

UCLASS()
class GAME_API ATrailActor : public AActor
{
	GENERATED_BODY()

	UFUNCTION()
	void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
						  const FHitResult& SweepResult);
	UFUNCTION()
	void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess=true))
	UStaticMeshComponent* StaticMesh;

	
protected:

	virtual void BeginPlay() override;

public:	

	ATrailActor();
	
	UPROPERTY(Replicated)
	USphereComponent* Sphere;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Proximity Boost")
	float SphereRadius;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	// Timer handle for destruction
	FTimerHandle DestructionTimerHandle;

	// Function to destroy itself
	UFUNCTION()
	void DestroySelf();

};

