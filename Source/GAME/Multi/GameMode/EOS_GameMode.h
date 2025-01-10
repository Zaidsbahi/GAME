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
	
protected:

	FTimerHandle TrackTimerHandle; // Handle for controlling the timer

	UPROPERTY(Replicated)
	float ElapsedTime; // Tracks elapsed time in seconds

	// Function to update the timer every second
	void UpdateElapsedTime();

	TArray<FName> TrackLevels; // List of track level names
	int32 CurrentTrackIndex;  // Current track index



};


