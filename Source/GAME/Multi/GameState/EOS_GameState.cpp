#include "EOS_GameState.h"
#include "Net/UnrealNetwork.h"

void AEOS_GameState::ClientSyncElapsedTime_Implementation(float Time)
{
	ElapsedTime = Time;
}

void AEOS_GameState::ClientSyncCountdownTime_Implementation(float Time)
{
	CountdownTime = Time;
}

void AEOS_GameState::OnRep_CollectiblesCount()
{
	UE_LOG(LogTemp, Log, TEXT("Collectibles count updated: %d"), CollectiblesCount);
}

void AEOS_GameState::AddCollectible()
{
	if (HasAuthority())
	{
		CollectiblesCount++;

		// Notify clients
		OnRep_CollectiblesCount();
	}
}

void AEOS_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate the elapsed time to all clients
	DOREPLIFETIME(AEOS_GameState, ElapsedTime);

	// Replicate the countdown time to all clients
	DOREPLIFETIME(AEOS_GameState, CountdownTime);

	DOREPLIFETIME(AEOS_GameState, CollectiblesCount);
}
