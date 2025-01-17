#include "EOS_GameMode.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GAME/Multi/GameInstance/EOS_GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Net/UnrealNetwork.h"
#include "GAME/Multi/GameState/EOS_GameState.h"
#include "GameFramework/PlayerState.h"


AEOS_GameMode::AEOS_GameMode()
{
	GameStateClass = AEOS_GameState::StaticClass();

	// Add track levels to the list
	TrackLevels = {TEXT("Track1"), TEXT("Track2"), TEXT("Track3") , TEXT("Track4")};
	CurrentTrackIndex = 0; // Start with the first track
}

void AEOS_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (NewPlayer)
	{
		FUniqueNetIdRepl UniqueNetIdRepl;
		if (NewPlayer->IsLocalController())
		{
			ULocalPlayer *LocalPlayerRef = NewPlayer->GetLocalPlayer();
			if (LocalPlayerRef)
			{
				UniqueNetIdRepl = LocalPlayerRef->GetPreferredUniqueNetId();
			}
			else
			{
				UNetConnection *RemoteNetConnectionRef = Cast<UNetConnection>(NewPlayer->Player);
				check(IsValid(RemoteNetConnectionRef));
				UniqueNetIdRepl = RemoteNetConnectionRef->PlayerId;
			}
		}
		else
		{
			UNetConnection *RemoteNetConnectionRef = Cast<UNetConnection>(NewPlayer->Player);
			check(IsValid(RemoteNetConnectionRef));
			UniqueNetIdRepl = RemoteNetConnectionRef->PlayerId;
		}

		TSharedPtr<const FUniqueNetId> UniqueNetId = UniqueNetIdRepl.GetUniqueNetId();
		if (!UniqueNetId.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("UniqueNetId is nullptr. Aborting session registration."));
			return;
		}
		IOnlineSubsystem *SubsystemRef = Online::GetSubsystem(NewPlayer->GetWorld());
		if (!SubsystemRef)
		{
			UE_LOG(LogTemp, Error, TEXT("OnlineSubsystem is nullptr."));
			return;
		}
		IOnlineSessionPtr SessionRef = SubsystemRef->GetSessionInterface();
		if (!SessionRef.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Session interface is invalid."));
			return;
		}
		bool bRegistrationSuccess = SessionRef->RegisterPlayer(FName("MainSession"), *UniqueNetId, false);
		if (bRegistrationSuccess)
		{
			UE_LOG(LogTemp, Warning, TEXT("Successfully registered the session"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to register the session."));
		}
	}
	
}

void AEOS_GameMode::RestartCurrentLevel()
{
	// Check if the world exists
	if (GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("RestartCurrentLevel called on the server."));
		// Use the OpenLevelText stored in your game instance
		UEOS_GameInstance* GameInstance = Cast<UEOS_GameInstance>(GetGameInstance());
		if (GameInstance)
		{
			FString CurrentLevelName = GameInstance->OpenLevelText;

			if (!CurrentLevelName.IsEmpty())
			{
				UE_LOG(LogTemp, Warning, TEXT("Restarting level: %s"), *CurrentLevelName);
				// Restart the level by using ServerTravel with the level name
				GetWorld()->ServerTravel(CurrentLevelName);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Level name is empty! Cannot restart the level."));
			}
		}
	}
}

void AEOS_GameMode::StartTrackTimer()
{
	GetWorld()->GetTimerManager().SetTimer(TrackTimerHandle, this, &AEOS_GameMode::UpdateElapsedTime, 1.0f, true);
}

void AEOS_GameMode::StopTrackTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TrackTimerHandle);
	UE_LOG(LogTemp, Log, TEXT("Track timer stopped. Elapsed Time: %f seconds"), ElapsedTime);

}

void AEOS_GameMode::BeginPlay()
{
	Super::BeginPlay();

	// Start the track timer when the game begins
	StartTrackTimer();
	UE_LOG(LogTemp, Log, TEXT("Track timer started in BeginPlay."));

	UEOS_GameInstance* GameInstance = Cast<UEOS_GameInstance>(GetGameInstance());
	if (GameInstance)
	{
		CurrentTrackIndex = GameInstance->CurrentTrackIndex;
		UE_LOG(LogTemp, Log, TEXT("GameMode: Retrieved CurrentTrackIndex = %d from GameInstance"), CurrentTrackIndex);
	}
}

void AEOS_GameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate the elapsed time to clients
	DOREPLIFETIME(AEOS_GameMode, ElapsedTime);
}

void AEOS_GameMode::UpdateElapsedTime()
{
	// Get the GameState
	AEOS_GameState* GameStateRef = GetGameState<AEOS_GameState>();
	if (GameState)
	{
		GameStateRef->ElapsedTime += 1.0f;
	}
}

void AEOS_GameMode::CompleteTrack()
{
	// Stop the timer
	GetWorld()->GetTimerManager().ClearTimer(TrackTimerHandle);

	// Log the final elapsed time
	if (AEOS_GameState* GameStateRef = GetGameState<AEOS_GameState>())
	{
		UE_LOG(LogTemp, Log, TEXT("Track completed in %f seconds"), GameStateRef->ElapsedTime);
	}

	// Transition to the next track after 5 seconds
	FTimerHandle TransitionHandle;
	GetWorld()->GetTimerManager().SetTimer(TransitionHandle, this, &AEOS_GameMode::LoadNextTrack, 5.0f, false);

}

void AEOS_GameMode::LoadNextTrack()
{
	if (CurrentTrackIndex + 1 < TrackLevels.Num())
	{

		UE_LOG(LogTemp, Log, TEXT("LoadNextTrack: CurrentTrackIndex before increment: %d"), CurrentTrackIndex);
		
		CurrentTrackIndex++;

		// Save the updated CurrentTrackIndex back to GameInstance:
		UEOS_GameInstance* GameInstance = Cast<UEOS_GameInstance>(GetGameInstance());
		if (GameInstance)
		{
			GameInstance->CurrentTrackIndex = CurrentTrackIndex;
		}

		FString NextTrackName = TrackLevels[CurrentTrackIndex].ToString();

		if (HasAuthority()) // Ensure this is called on the server
		{
			FString TravelCommand = FString::Printf(TEXT("%s?listen"), *NextTrackName);
			GetWorld()->ServerTravel(TravelCommand, false, false); // Use ServerTravel to maintain multiplayer state
			UE_LOG(LogTemp, Log, TEXT("Loading next track: %s with ServerTravel"), *NextTrackName);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("LoadNextTrack called on the client. This should only happen on the server."));
		}
		
		//UGameplayStatics::OpenLevel(GetWorld(), TrackLevels[CurrentTrackIndex]);
		//UE_LOG(LogTemp, Log, TEXT("Loading next track: %s"), *TrackLevels[CurrentTrackIndex].ToString());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("No more tracks. Game complete!"));
		// TODO: End game logic
	}
}

void AEOS_GameMode::RestartCurrentTrack()
{
	FString CurrentLevelName = GetWorld()->GetMapName();
	CurrentLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix); // Remove prefix if any

	// Unpause the game to reset input mode
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->SetPause(false);
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());
	}
	
	FString TravelCommand = FString::Printf(TEXT("%s?listen"), *CurrentLevelName);
	GetWorld()->ServerTravel(TravelCommand, false, false);
}

void AEOS_GameMode::LoadMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), "ZaidTestStart"); // Replace "MainMenu" with the actual level name
}
