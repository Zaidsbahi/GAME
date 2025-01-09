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

	void StopTrackTimer();

	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Timer")
	float GetElapsedTime() const { return ElapsedTime; }
	
protected:

	FTimerHandle TrackTimerHandle; // Handle for controlling the timer

	UPROPERTY(Replicated)
	float ElapsedTime; // Tracks elapsed time in seconds

	// Function to update the timer every second
	void UpdateElapsedTime();


};
