#include "EOSGameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Online/OnlineSessionNames.h"

void UEOSGameInstance::JoinLobbyBySearchResultIndex(int Index)
{
	FUniqueNetIdPtr UserId = IdentityPtr->GetUniquePlayerId(0);
	if (!UserId.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot join lobby. Invalid LocalUserId."));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Joining lobby with LocalUserId: %s"), *UserId->ToString());

	if (Index < 0 || Index >= SessionSearch->SearchResults.Num())
	{
		return;
	}

	const auto& SearchResult = SessionSearch->SearchResults[Index];

	FString SessionName = GetSessionName(SearchResult);
	
	if (SessionPtr)
	{
		SessionPtr->JoinSession(0,FName(SessionName), SearchResult);
	}
	
}

void UEOSGameInstance::Login()
{
	if (!IdentityPtr)
	{
		UE_LOG(LogTemp, Error, TEXT("IdentityPtr is null! Cannot log in."));
		return;
	}
	
		FOnlineAccountCredentials OnlineAccountCredentials;
		OnlineAccountCredentials.Type = TEXT("accountportal");
		OnlineAccountCredentials.Id = TEXT("");
		OnlineAccountCredentials.Token = TEXT("");

		UE_LOG(LogTemp, Warning, TEXT("Attempting to log in..."));
	
		IdentityPtr->Login(0, OnlineAccountCredentials);
	
}

void UEOSGameInstance::CreateSession(const FName& SessionName)
{
	FUniqueNetIdPtr UserId = IdentityPtr->GetUniquePlayerId(0);
	if (!UserId.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot create session. Invalid LocalUserId."));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Creating session with LocalUserId: %s"), *UserId->ToString());

	if (!SessionPtr)
	{
		UE_LOG(LogTemp, Error, TEXT("SessionPtr is null! Cannot create session."));
		return;
	}
	
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bAllowInvites = true;
		SessionSettings.bIsDedicated = false;
		SessionSettings.bIsLANMatch = false;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUseLobbiesIfAvailable = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.bAllowJoinInProgress = true;
		SessionSettings.bAllowJoinViaPresence = true;
		SessionSettings.NumPublicConnections = 10;

		//Passing an Arbitrary Data, and make it available to be read on the client.
		SessionSettings.Set(SessionNameKey, SessionName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		UE_LOG(LogTemp, Warning, TEXT("Creating session with name: %s"), *SessionName.ToString());
		
		SessionPtr->CreateSession(0, SessionName,SessionSettings);
	
}

void UEOSGameInstance::FindSession()
{
	FUniqueNetIdPtr UserId = IdentityPtr->GetUniquePlayerId(0);
	if (!UserId.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot find session. Invalid LocalUserId."));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Finding sessions with LocalUserId: %s"), *UserId->ToString());

	if (!SessionPtr)
	{
		UE_LOG(LogTemp, Error, TEXT("SessionPtr is null! Cannot find sessions."));
		return;
	}
	
	SessionSearch = MakeShareable(new FOnlineSessionSearch);


		if (!SessionSearch.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("SessionSearch is invalid"));
			return;
		}
			
		SessionSearch->bIsLanQuery = false;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_KEYWORDS, true, EOnlineComparisonOp::Equals);

	UE_LOG(LogTemp, Warning, TEXT("Search query prepared. Searching for sessions..."));
		
		SessionPtr->FindSessions(0, SessionSearch.ToSharedRef());
	
}

FString UEOSGameInstance::GetSessionName(const FOnlineSessionSearchResult& SearchResult) const
{
	FString OutValue = TEXT("Name_None");
	if (!SearchResult.Session.SessionSettings.Get(SessionNameKey, OutValue))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to retrieve session name. Using default name: %s"), *OutValue);
	}
	return OutValue;
}

void UEOSGameInstance::Init()
{
	Super::Init();
	OnlineSubsystem = IOnlineSubsystem::Get();
	IdentityPtr = OnlineSubsystem->GetIdentityInterface();

	if (IdentityPtr)
	{
		FUniqueNetIdPtr UserId = IdentityPtr->GetUniquePlayerId(0);
		if (UserId.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Initialized with LocalUserId: %s"), *UserId->ToString());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to retrieve LocalUserId during initialization."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("IdentityPtr is null!"));
	}

	SessionPtr = OnlineSubsystem->GetSessionInterface();
	SessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::CreateSessionComplete);
	SessionPtr->OnFindSessionsCompleteDelegates.AddUObject(this, &UEOSGameInstance::FindSessionComplete);
	SessionPtr->OnJoinSessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::JoinSessionComplete);

	}

void UEOSGameInstance::LoginComplete(int NumOfPlayers, bool bWasSuccessful, const FUniqueNetId& UserId,
	const FString& Error)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Login successful! User ID: %s"), *UserId.ToString());
		FUniqueNetIdPtr RetrievedUserId = IdentityPtr->GetUniquePlayerId(0);
		if (RetrievedUserId.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Retrieved LocalUserId: %s"), *RetrievedUserId->ToString());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to retrieve LocalUserId after login."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Login failed: %s"), *Error);
	}
}

void UEOSGameInstance::CreateSessionComplete(FName name, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session created Successfully"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Session created Failed"));
	}

	if (!GameLevel.IsValid())
	{
		GameLevel.LoadSynchronous();
	}
	if (GameLevel.IsValid())
	{
		const FName LevelName = FName(*FPackageName::ObjectPathToPackageName(GameLevel.ToString()));
		GetWorld()->ServerTravel(LevelName.ToString() + "?listen");
	}
}

void UEOSGameInstance::FindSessionComplete(bool bWasSuccessful)
{
	if (!SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("SessionSearch is no longer valid."));
		return;
	}

	if (bWasSuccessful && SessionSearch->SearchResults.Num() > 0)
	{
		for (int32 i = 0; i < SessionSearch->SearchResults.Num(); ++i)
		{
			const auto& SearchResult = SessionSearch->SearchResults[i];
			FString LobbyName = GetSessionName(SearchResult);
			UE_LOG(LogTemp, Warning, TEXT("Found Session [%d]: ID=%s, Name=%s"), 
				   i, *SearchResult.GetSessionIdStr(), *LobbyName);
		}

		SearchCompleted.Broadcast(SessionSearch->SearchResults);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("FindSessionComplete: No sessions found or search failed."));
	}
}

void UEOSGameInstance::JoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		FString TravelUrl;
		if (SessionPtr->GetResolvedConnectString(SessionName, TravelUrl))
		{
			UE_LOG(LogTemp, Warning, TEXT("Joining session succeeded. Traveling to URL: %s"), *TravelUrl);
			GetFirstLocalPlayerController(GetWorld())->ClientTravel(TravelUrl, ETravelType::TRAVEL_Absolute);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to resolve connect string for session: %s"), *SessionName.ToString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Join session failed for session: %s. Result: %d"), *SessionName.ToString(), static_cast<int32>(Result));
	}
}
