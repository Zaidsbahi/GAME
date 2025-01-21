#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "EOS_GameState.generated.h"


UCLASS()
class GAME_API AEOS_GameState : public AGameStateBase
{
	GENERATED_BODY()


public:
	// Elapsed time variable (replicated to clients)
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Timer")
	float ElapsedTime;

	// Getter function for the elapsed time
	UFUNCTION(BlueprintCallable, Category = "Timer")
	float GetElapsedTime() const { return ElapsedTime; }

	UFUNCTION(Client, Reliable)
	void ClientSyncElapsedTime(float Time);

protected:
	// Replication setup
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
