#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GAME/ProximityBoost/ProximityBoost_Component.h"
#include "PlayerCharacter_Base.generated.h"

UCLASS()
class GAME_API APlayerCharacter_Base : public ACharacter
{

	GENERATED_BODY()
	
	///// Timer /////
	FTimerHandle AirDashTimerHandle;

	////////////////////////////////
	/// Inputs & MappingContext  ///
	////////////////////////////////
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AirDash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input",meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> MechanicsMappingContext;

	UPROPERTY()
	UProximityBoost_Component* ProximityBoostComponent;

protected:

	virtual void BeginPlay() override;

	////////////////////////////////
	//////  JUMP Variables  ////////
	////////////////////////////////
	
	// Track the maximum jump count based on pickups
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Jump")
	int32 MaxJumpCount;
	// Current jumps remaining during a jump sequence
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Jump")
	int32 CurrentJumpCount;
	// Flag to track if the character is on the ground
	bool bIsGrounded;

	
	////////////////////////////////
	//////  Dash Variables  ////////
	////////////////////////////////
	//AirDash Duration
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AirDash")
	float AirDashDuration;
	// ?bIsAirDashing is the Character AirDashing
	UPROPERTY(Replicated)
	bool bIsAirDashing;
	
public:

	APlayerCharacter_Base();

	////////////////////////////////
	/////  Replication setup  //////
	////////////////////////////////
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	////////////////////////////////
	//////  JUMP Functions  ////////
	////////////////////////////////
	
	// Function to handle jumps
	virtual void Jump() override;
	UFUNCTION(BlueprintCallable, Category = "Jump")
	void ResetJumpCount();
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void PrintJumpCount();

	///////////////////////////////
	////  AirDash Functions ///////
	///////////////////////////////
	
	// AirDash Normal Function
	void PerformAirDash();
	// AirDash Server Function
	UFUNCTION(Server, Reliable)
	void PerformAirDash_Server();
	// AirDash NetMulticast Function
	UFUNCTION(NetMulticast, Reliable)
	void PerformAirDash_NetMulticast();
	// AirDash End
	void EndAirDash();
	//AirDash Speed
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "AirDash")
	float AirDashSpeed;

	//////////////////////////////
	//////  Pickup LOGIC  ////////
	//////////////////////////////
	
	// Function to collect pickups and increment jump count
	UFUNCTION(Server, Reliable, WithValidation)
	void CollectPickup();

	//////////////////////////////////////
	//////  ProximityBoost LOGIC  ////////
	//////////////////////////////////////
	UFUNCTION()
	void EnableSuperJump(bool bEnable);
	
	//////////////////////////////
	//////  Landed & inLine //////
	//////////////////////////////

	virtual void Landed(const FHitResult& Hit) override;
};

inline void APlayerCharacter_Base::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	bIsGrounded = true;
}


