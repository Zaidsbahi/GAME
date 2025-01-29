#include "EOS_GameState.h"

#include "GAME/PlayerCharacter/PlayerCharacter_Base.h"
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

///////////////////////////
///   Winning Condition ///
///////////////////////////
void AEOS_GameState::MulticastShowWinningScreen_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] MulticastShowWinningScreen Called!"), HasAuthority() ? TEXT("Server") : TEXT("Client"));

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		if (!PlayerController)
		{
			UE_LOG(LogTemp, Error, TEXT("PlayerController is nullptr! Skipping..."));
			continue;
		}

		APlayerCharacter_Base* PlayerCharacter = Cast<APlayerCharacter_Base>(PlayerController->GetPawn());
		if (!PlayerCharacter)
		{
			UE_LOG(LogTemp, Error, TEXT("PlayerCharacter is nullptr for controller: %s. Skipping..."), *PlayerController->GetName());
			continue;
		}

		UE_LOG(LogTemp, Warning, TEXT("[%s] Found Player Character: %s"),
			   HasAuthority() ? TEXT("Server") : TEXT("Client"),
			   *PlayerCharacter->GetName());

		// Add UI on **all** clients
		PlayerCharacter->AddWinningWidget();
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

	DOREPLIFETIME(AEOS_GameState, bShouldJog);
}
