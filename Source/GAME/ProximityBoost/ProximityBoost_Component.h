#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ProximityBoost_Component.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAME_API UProximityBoost_Component : public UActorComponent
{
	GENERATED_BODY()

	/////////////////////////////////////////////
	////  Timer & SphereOverlap Begin & End  ////
	/////////////////////////////////////////////
	FTimerHandle ProximityEffectTimer;
	UFUNCTION()
	void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
							  const FHitResult& SweepResult);
	UFUNCTION()
	void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnRep_ProximityState();

	UPROPERTY()
	USphereComponent* Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess=true))
	UStaticMeshComponent* StaticMesh;
	
protected:

	virtual void BeginPlay() override;

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	////////////////////////////////////////////////////
	////  Variables & Functions for ProximityBoost  ////
	////////////////////////////////////////////////////
	UProximityBoost_Component();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proximity Boost")
	float SphereRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proximity Boost")
	float EffectDuration;
	UPROPERTY(ReplicatedUsing=OnRep_ProximityState)
	bool bCanInfiniteDashAndDoubleJump;
	//Functions
	void ActivateProximityBoost();
	void DeactivateProximityBoost();
};
