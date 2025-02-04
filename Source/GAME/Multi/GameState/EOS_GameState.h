#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "EOS_GameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCollectiblesUpdated, int32, CollectiblesCount);

UCLASS()
class GAME_API AEOS_GameState : public AGameStateBase
{
	GENERATED_BODY()


public:
	// Elapsed time variable (replicated to clients)
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Timer")
	float ElapsedTime;

	// Countdown time variable (replicated to clients)
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Timer")
	float CountdownTime;

	// Getter function for the elapsed time
	UFUNCTION(BlueprintCallable, Category = "Timer")
	float GetElapsedTime() const { return ElapsedTime; }
	
	// Getter function for the countdown time
	UFUNCTION(BlueprintCallable, Category = "Timer")
	float GetCountdownTime() const { return CountdownTime; }

	UFUNCTION(Client, Reliable)
	void ClientSyncElapsedTime(float Time);

	// Function to sync countdown time on clients
	UFUNCTION(Client, Reliable)
	void ClientSyncCountdownTime(float Time);

	////////////////////
	/// Collectibles ///
	////////////////////
	
	// Shared collectible count (replicated)
	UPROPERTY(ReplicatedUsing = OnRep_CollectiblesCount, BlueprintReadOnly, Category = "Collectibles")
	int32 CollectiblesCount = 0;

	// Replication function
	UFUNCTION()
	void OnRep_CollectiblesCount();

	// Function to increase collectibles count
	UFUNCTION(BlueprintCallable, Category = "Collectibles")
	void AddCollectible();

	// Function to get collectible count
	UFUNCTION(BlueprintCallable, Category = "Collectibles")
	int32 GetCollectibleCount() const { return CollectiblesCount; }

	UPROPERTY(Replicated)
	bool bShouldJog = false;

	///////////////////////////
	///   Winning Condition ///
	///////////////////////////
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void MulticastShowWinningScreen();

	///////////////////////////
	///   Collectables UI   ///
	///////////////////////////
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCollectiblesUpdated OnCollectiblesUpdated;

protected:
	// Replication setup
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
