#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EOS_GameMode.generated.h"


UCLASS()
class GAME_API AEOS_GameMode : public AGameModeBase
{
	GENERATED_BODY()

	AEOS_GameMode();

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION(BlueprintCallable, Category = "Game")
	void RestartCurrentLevel();

	// Function to start the timer
	void StartTrackTimer();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void StopTrackTimer();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Timer")
	float GetElapsedTime() const { return ElapsedTime; }

	UFUNCTION(BlueprintCallable, Category = "Timer")
	void CompleteTrack();
	
	void LoadNextTrack();
	
	UFUNCTION(BlueprintCallable, Category = "Game")
	void RestartCurrentTrack();
	
	UFUNCTION(BlueprintCallable, Category = "Game")
	void LoadMainMenu();

	float LastTime = 0.0f;

	///////////////////////////
	/// Ready-Up Mechanism ////
	///////////////////////////

	// Widget class to use for the Ready-Up UI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> ReadyWidgetClass;

	// Instance of the widget
	UPROPERTY()
	UUserWidget* ReadyWidgetInstance;

	// Tracks the number of players currently ready
	int32 PlayersReadyCount;

	// Called when a player's ready status changes
	UFUNCTION()
	void OnPlayerReadyChanged(bool bIsReady);

	// Function to check if the game should start
	void CheckReadyState();

	UFUNCTION(BlueprintCallable, Category = "Game")
	bool ArePlayersReady() const;

	//UPROPERTY(ReplicatedUsing=OnRep_ShouldJog)
	//bool bShouldJog = false;

	UFUNCTION()
	void OnRep_ShouldJog();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartJogging();

	// Function to start gameplay
	void StartGameplay();

	// Disable player movement
	void DisablePlayerMovement();

	// Enable player movement
	void EnablePlayerMovement();
	

	/////////////////////
	/// Collectibles ////
	/////////////////////

	// Required collectibles to proceed
	UPROPERTY(EditDefaultsOnly, Category = "Game|Collectibles")
	int32 RequiredCollectibles = 6;

	// Function to check if players can progress
	void CheckCollectibleRequirement();

protected:

	///////////////////////////
	/// Timer Configuration ///
	///////////////////////////

	FTimerHandle TrackTimerHandle; // Handle for controlling the timer

	UPROPERTY(Replicated)
	float CountdownTime; // Countdown duration in seconds (e.g., 60 seconds)

	UPROPERTY(EditDefaultsOnly, Category = "Game|Timer")
	float InitialCountdownTime = 90.0f; // Default starting countdown time

	// Function to update the timer every second
	void UpdateCountdownTime();

	
	UPROPERTY(Replicated)
	float ElapsedTime; // Tracks elapsed time in seconds

	// Function to update the timer every second
	void UpdateElapsedTime();

	TArray<FName> TrackLevels; // List of track level names
	UPROPERTY(Replicated)
	int32 CurrentTrackIndex;  // Current track index



};


