#include "EOS_GameMode.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GAME/Multi/GameInstance/EOS_GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"

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


