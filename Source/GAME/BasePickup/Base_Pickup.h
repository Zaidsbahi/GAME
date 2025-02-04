#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Base_Pickup.generated.h"

UCLASS()
class GAME_API ABase_Pickup : public AActor
{
	GENERATED_BODY()


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> SphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components",meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Mesh;

	FTimerHandle RespawnTimerHandle; // Handle for managing the respawn timer

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor*
	OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool
	bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category ="Pickup", meta=(DisplayName="Pickup"))
	void Pickup(class ACharacter* OwningCharacter);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup")
	float RespawnTime;

	virtual void Pickup_Implementation(class ACharacter* OwningCharacter);


public:

	ABase_Pickup();

	virtual void Tick(float DeltaTime) override;

	FORCEINLINE USphereComponent* GetSphereCollision() const { return SphereCollision; }
    FORCEINLINE UStaticMeshComponent* GetMesh() const { return Mesh; }


	
	UFUNCTION(BlueprintCallable)
	void RespawnPickup();

	///////////////////////////
	///       Sounds        ///
	///////////////////////////
	UFUNCTION(BlueprintImplementableEvent, Category = "Audio")
	void PlayPickupSound(int32 PickupIndex);

	///////////////////////////
	///       Respawning    ///
	///////////////////////////
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= "Pickup")
	bool bShouldRespawn;
};
