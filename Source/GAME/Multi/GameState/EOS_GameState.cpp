#include "EOS_GameState.h"
#include "Net/UnrealNetwork.h"

void AEOS_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate the elapsed time to all clients
	DOREPLIFETIME(AEOS_GameState, ElapsedTime);
}
