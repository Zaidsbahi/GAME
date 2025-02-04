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
	UPROPERTY(Replicated)
	bool bIsActivatedProximity;

	
	//Functions
	void ActivateProximityBoost();
	void DeactivateProximityBoost();
	bool IsProximityBoostActive() const;

	UPROPERTY()
	USphereComponent* Sphere;

	UFUNCTION()
	void ActivateProximityOnBeginOverlap();
	UFUNCTION()
	void DeActivateProximityOnEndOverlap();


	//////////////////////////////////////
	/////  Trail Actor Server Logic //////
	//////////////////////////////////////

	// Activate Proximity Boost
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerActivateProximityBoost();
	void ServerActivateProximityBoost_Implementation();
	bool ServerActivateProximityBoost_Validate();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastActivateProximityBoost();

	// Deactivate Proximity Boost
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDeActivateProximityBoost();
	void ServerDeActivateProximityBoost_Implementation();
	bool ServerDeActivateProximityBoost_Validate();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastDeActivateProximityBoost();

	///////////////////////
	/////  Materials //////
	///////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* ActiveMaterial;  // Green material when activated
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* InactiveMaterial;  // Red material when deactivated
	
};
