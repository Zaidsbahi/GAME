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

	////  PRIVATE  ////
	
	/////////////////////////////////////////////
	////  Timer & SphereOverlap Begin & End  ////
	/////////////////////////////////////////////
	FTimerHandle ProximityEffectTimer;
	UFUNCTION()
	void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
							  const FHitResult& SweepResult);
	UFUNCTION()
	void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/////////////////////////////////////////////////////////////////
	////  OnRepProximityState & Sphere + Static Mesh Components  ////
	/////////////////////////////////////////////////////////////////
	UFUNCTION()
	void OnRep_ProximityState();
	UPROPERTY()
	USphereComponent* Sphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess=true))
	UStaticMeshComponent* StaticMesh;
	
	////  PROTECTED  ////
protected:

	// Begin Play
	virtual void BeginPlay() override;

	////  PUBLIC  ////
public:

	//////////////////////////////////
	////  Tick & ReplicatedProps  ////
	//////////////////////////////////
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	///////////////////////////////////////////////
	////  Variables & Functions & Constructor  ////
	///////////////////////////////////////////////
	//Constructor
	UProximityBoost_Component();
	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proximity Boost")
	float SphereRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proximity Boost")
	float EffectDuration;
	UPROPERTY(ReplicatedUsing=OnRep_ProximityState)
	bool bCanInfiniteDashAndDoubleJump;
	UPROPERTY(Replicated)
	bool bIsStillInRangeOfProximity;
	//Functions
	void ActivateProximityBoost();
	void DeactivateProximityBoost();
};
