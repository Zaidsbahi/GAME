#include "EOS_GameMode.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GAME/Multi/GameInstance/EOS_GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Net/UnrealNetwork.h"
#include "GAME/Multi/GameState/EOS_GameState.h"
#include "GAME/Multi/PlayerController/EOS_PlayerController.h"
#include "GAME/PlayerState/PlayerState_Base.h"
#include "GameFramework/PlayerState.h"


AEOS_GameMode::AEOS_GameMode()
{
	GameStateClass = AEOS_GameState::StaticClass();

	// Add track levels to the list
	TrackLevels = {TEXT("Track1"), TEXT("Track2"), TEXT("Track3") , TEXT("Track4")};
	CurrentTrackIndex = 1; // Start with the first track

	ElapsedTime = 0.0f;
	ReadyWidgetInstance = nullptr;
	PlayersReadyCount = 0; // No players are ready at the start
	
	bReplicates = true;
}

void AEOS_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// Get the current level name
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	// Skip Ready-Up system for Track1 (tutorial level)
	if (CurrentLevelName == "Track1")
	{
		UE_LOG(LogTemp, Log, TEXT("Skipping Ready-Up system for Track1."));
		return;
	}
	
	// Show the Ready-Up widget for the newly joined player
	if (AEOS_PlayerController* PlayerController = Cast<AEOS_PlayerController>(NewPlayer))
	{
		PlayerController->ShowReadyWidget(ReadyWidgetClass); // Ensure widget is shown
		PlayerController->DisableInputForPlayer(); // Disable input for the player
	}

	// Bind the ready status delegate for the new player's PlayerState
	if (APlayerState_Base* PlayerState = NewPlayer->GetPlayerState<APlayerState_Base>())
	{
		PlayerState->OnReadyChanged.AddDynamic(this, &AEOS_GameMode::OnPlayerReadyChanged);
	}

	// Log the current number of connected players
	UE_LOG(LogTemp, Log, TEXT("Player joined. Current number of players: %d"), GameState->PlayerArray.Num());

	
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
		
		// Synchronize the elapsed time for newly joined clients
		if (HasAuthority())
		{
			if (AEOS_GameState* GameStateRef = GetGameState<AEOS_GameState>())
			{
				GameStateRef->ElapsedTime = FMath::Max(GameStateRef->ElapsedTime, 0.0f);
				GameStateRef->ClientSyncElapsedTime(GameStateRef->ElapsedTime);
				UE_LOG(LogTemp, Log, TEXT("Sent elapsed time to client: %f"), GameStateRef->ElapsedTime);
			}
		}
		
	}
}

void AEOS_GameMode::RestartCurrentLevel()
{
	// Check if the world exists
	if (GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("RestartCurrentLevel called on the server."));

		if (AEOS_GameState* GameStateRef = GetGameState<AEOS_GameState>())
		{
			GameStateRef->ElapsedTime = 0.0f;
		}
		
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
	GetWorld()->GetTimerManager().SetTimer(TrackTimerHandle, this, &AEOS_GameMode::UpdateElapsedTime, 0.01f, true);

	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(TrackTimerHandle, this, &AEOS_GameMode::UpdateCountdownTime, 1.0f, true);
		UE_LOG(LogTemp, Log, TEXT("Countdown timer started."));
	}
}

void AEOS_GameMode::StopTrackTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TrackTimerHandle);
	UE_LOG(LogTemp, Log, TEXT("Track timer stopped. Elapsed Time: %f seconds"), ElapsedTime);

	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().ClearTimer(TrackTimerHandle);
		UE_LOG(LogTemp, Log, TEXT("Countdown timer stopped."));
	}

}

void AEOS_GameMode::BeginPlay()
{
	Super::BeginPlay();

	// Initialize the countdown timer
	if (HasAuthority()) 
	{
		CountdownTime = InitialCountdownTime;

		// Sync the countdown time to the GameState
		if (AEOS_GameState* GameStateRef = GetGameState<AEOS_GameState>())
		{
			GameStateRef->CountdownTime = CountdownTime;
			GameStateRef->ClientSyncCountdownTime(CountdownTime);
		}

		UE_LOG(LogTemp, Log, TEXT("Countdown timer initialized: %.2f seconds"), CountdownTime);
	}

	// Start the track timer when the game begins
	if (HasAuthority()) 
	{
		if (AEOS_GameState* GameStateRef = GetGameState<AEOS_GameState>())
		{
			GameStateRef->ElapsedTime = 0.0f;  // Reset the timer at start
		}
		
		/*
		if (!GetWorld()->GetTimerManager().IsTimerActive(TrackTimerHandle))
		{
			StartTrackTimer();
			UE_LOG(LogTemp, Log, TEXT("Track timer started in BeginPlay."));
		}
		*/
	}

	UEOS_GameInstance* GameInstance = Cast<UEOS_GameInstance>(GetGameInstance());
	if (GameInstance)
	{
		CurrentTrackIndex = GameInstance->CurrentTrackIndex;
		UE_LOG(LogTemp, Log, TEXT("GameMode: Retrieved CurrentTrackIndex = %d from GameInstance"), CurrentTrackIndex);
	}

	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	if (CurrentLevelName != "Track1" && ReadyWidgetClass) // Skip Track1 (tutorial)
	{
		// Notify all players to show the Ready-Up widget
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			if (AEOS_PlayerController* PC = Cast<AEOS_PlayerController>(It->Get()))
			{
				PC->ShowReadyWidget(ReadyWidgetClass);
				PC->DisableInputForPlayer(); // Disable movement for all players
			}
		}

		// Bind the ready status change delegate for each player's PlayerState
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			if (APlayerController* PC = It->Get())
			{
				if (APlayerState_Base* PlayerState = PC->GetPlayerState<APlayerState_Base>())
				{
					PlayerState->OnReadyChanged.AddDynamic(this, &AEOS_GameMode::OnPlayerReadyChanged);
				}
			}
		}
	}
	
}

void AEOS_GameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate the elapsed time to clients
	DOREPLIFETIME(AEOS_GameMode, ElapsedTime);
	DOREPLIFETIME(AEOS_GameMode, CurrentTrackIndex);
	DOREPLIFETIME(AEOS_GameMode, CountdownTime);
}

void AEOS_GameMode::UpdateCountdownTime()
{
	if (HasAuthority())
	{
		CountdownTime -= 1.0f;

		// Sync the countdown time to clients
		if (AEOS_GameState* GameStateRef = GetGameState<AEOS_GameState>())
		{
			GameStateRef->CountdownTime = CountdownTime;
			GameStateRef->ClientSyncCountdownTime(CountdownTime);
		}

		// Broadcast the remaining time (optional)
		UE_LOG(LogTemp, Log, TEXT("Time remaining: %.2f seconds"), CountdownTime);

		// Check if the timer has reached zero
		if (CountdownTime <= 0.0f)
		{
			CountdownTime = 0.0f; // Clamp to zero
			StopTrackTimer();

			// Restart the track when the timer ends
			RestartCurrentTrack();
			UE_LOG(LogTemp, Warning, TEXT("Time is up! Restarting track."));
		}
	}
}

void AEOS_GameMode::UpdateElapsedTime()
{
	if (HasAuthority())  
	{
		float CurrentTime = GetWorld()->GetRealTimeSeconds();

		if (LastTime == 0.0f) 
		{
			LastTime = CurrentTime; // Initialize LastTime on the first run
		}

		float DeltaTime = CurrentTime - LastTime;
		LastTime = CurrentTime;

		if (AEOS_GameState* GameStateRef = GetGameState<AEOS_GameState>())
		{
			GameStateRef->ElapsedTime += DeltaTime;
		}
	}
}

void AEOS_GameMode::CompleteTrack()
{
	AEOS_GameState* GameStateRef = GetGameState<AEOS_GameState>();
	if (!GameStateRef || GameStateRef->GetCollectibleCount() < RequiredCollectibles)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough collectibles to proceed!"));
		return; // Prevent track from completing
	}

	// Reset collectible count for the next track
	GameStateRef->CollectiblesCount = 0;
	
	// Stop the timer
	StopTrackTimer();
	
	// Stop the timer
	GetWorld()->GetTimerManager().ClearTimer(TrackTimerHandle);

	// Log the completion and reset time
	if (AEOS_GameState* GameStateRefo = GetGameState<AEOS_GameState>())
	{
		UE_LOG(LogTemp, Log, TEXT("Track completed with %.2f seconds remaining."), CountdownTime);
		CountdownTime = InitialCountdownTime; // Reset the timer for the next track

		// Log the final elapsed time
		UE_LOG(LogTemp, Log, TEXT("Track completed in %f seconds"), GameStateRef->ElapsedTime);
		
		// Reset the elapsed time before transitioning to the next track
		GameStateRefo->ElapsedTime = 0.0f;
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
		// Handle end-of-game logic here
	}
}

void AEOS_GameMode::RestartCurrentTrack()
{
	FString CurrentLevelName = GetWorld()->GetMapName();
	CurrentLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix); // Remove prefix if any

	// Reset the countdown timer for the new track
	CountdownTime = InitialCountdownTime;
	
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



///////////////////////////
/// Ready-Up Mechanism ////
///////////////////////////

void AEOS_GameMode::OnPlayerReadyChanged(bool bIsReady)
{
	// Reset the ready count
	PlayersReadyCount = 0;

	// Count how many players are ready
	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		if (APlayerState_Base* BasePlayerState = Cast<APlayerState_Base>(PlayerState))
		{
			if (BasePlayerState->bIsReady)
			{
				PlayersReadyCount++;
			}
		}
	}

	// Check if the game should start
	CheckReadyState();
}
void AEOS_GameMode::CheckReadyState()
{
	if (PlayersReadyCount == 2) // Ensure both players are ready
	{
		StartGameplay();
	}
}
void AEOS_GameMode::StartGameplay()
{
	UE_LOG(LogTemp, Log, TEXT("Both players are ready. Starting the game!"));

	// Enable player movement
	EnablePlayerMovement();

	// Notify all players to remove the Ready-Up widget
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (AEOS_PlayerController* PC = Cast<AEOS_PlayerController>(It->Get()))
		{
			PC->RemoveReadyWidget();
		}
	}
	
	// Start the timer or gameplay logic
	StartTrackTimer();
}
void AEOS_GameMode::DisablePlayerMovement()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (AEOS_PlayerController* PC = Cast<AEOS_PlayerController>(It->Get()))
		{
			PC->DisableInputForPlayer(); // Call Client RPC to disable input
		}
	}
}
void AEOS_GameMode::EnablePlayerMovement()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (AEOS_PlayerController* PC = Cast<AEOS_PlayerController>(It->Get()))
		{
			PC->EnableInputForPlayer(); // Call Client RPC to enable input
		}
	}
}

void AEOS_GameMode::CheckCollectibleRequirement()
{
	AEOS_GameState* GameStateRef = GetGameState<AEOS_GameState>();
	if (GameStateRef && GameStateRef->GetCollectibleCount() >= RequiredCollectibles)
	{
		CompleteTrack(); // Progress the track
	}
}
