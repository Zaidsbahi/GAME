#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GAME/ProximityBoost/ProximityBoost_Component.h"
#include "PlayerCharacter_Base.generated.h"

UCLASS()
class GAME_API APlayerCharacter_Base : public ACharacter
{

	GENERATED_BODY()

	////  PRIVATE  ////
	
	////////////////////////////////////////
	/// Inputs & MappingContext & Timer  ///
	////////////////////////////////////////
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AirDash;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> RestartLevelAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> RestartTrackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Input",meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> MechanicsMappingContext;

	///// Timer /////
	FTimerHandle AirDashTimerHandle;



	////  Protected  ////
protected:

	// Begin Play
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
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Jump")
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

	////  PUBLIC  ////
public:

	// Constructor
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

	///Restart
	void RestartLevel();

	void RestartTrack();


	UPROPERTY()
	UProximityBoost_Component* ProximityBoostComponent;
	
	
	//////////////////////
	//////  Getters //////
	//////////////////////
	UFUNCTION()
	bool ReturnPlayerJumpActiveBool() const;
	UFUNCTION()
	bool ReturnPlayerDashActiveBool() const;

//	UFUNCTION(Blueprintable)
//	void CheckingActivateProximityFromCharacter();

	// Fixing the Landing Bug
	UFUNCTION(BlueprintCallable)
	void ResetJumpHeightAfterLanding();

	UFUNCTION()
	void ActivatePlayersProximityBoost();
	UFUNCTION()
	void DeActivatePlayersProximityBoost();
	UFUNCTION()
	void SpawnTrailActors();

	// Timer handle for continuous trail spawning
	FTimerHandle TrailSpawnTimerHandle;
    
	// Boolean to track if the trail should continue spawning
	bool bIsGeneratingTrail;

	// Function to spawn trail continuously
	void SpawnTrailRepeatedly();

	// Function to stop trail spawning
	UFUNCTION(BlueprintCallable)
	void StopTrailSpawning();

	////////////////
	///  Coyote  ///
	////////////////
	// Timer for Coyote Time
	FTimerHandle CoyoteTimeTimerHandle;
	// Boolean to track if Coyote Time is active
	bool bCanUseCoyoteTime;
	// Duration of Coyote Time (e.g., 0.2 seconds)
	float CoyoteTimeDuration = 0.4f;
	// Function to start the coyote time timer
	void StartCoyoteTime();
	// Function to disable coyote time after it expires
	void DisableCoyoteTime();

	// Jogging speed variable
	UPROPERTY(EditAnywhere, Category = "Movement")
	float JoggingSpeed = 250.0f; // Adjust based on animation speeds

	///////////////////////////
	///   Winning Condition ///
	///////////////////////////
	UFUNCTION(BlueprintCallable)
	void AddWinningWidget();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> WinningWidgetClass;
};


inline void APlayerCharacter_Base::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	bIsGrounded = true;
	StopTrailSpawning();
	UE_LOG(LogTemp, Log , TEXT("Landed Bro!"))

	// Reset coyote time availability
	bCanUseCoyoteTime = false;
	GetWorld()->GetTimerManager().ClearTimer(CoyoteTimeTimerHandle);
}


