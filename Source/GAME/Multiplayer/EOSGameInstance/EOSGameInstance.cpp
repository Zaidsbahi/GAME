#include "EOSGameInstance.h"
#include "OnlineSubSystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Online/OnlineSessionNames.h"

void UEOSGameInstance::Login()
{
	if (IdentityPtr)
	{
		FOnlineAccountCredentials OnlineAccountCredentials;
		OnlineAccountCredentials.Type = "accountportal";
		OnlineAccountCredentials.Id = "";
		OnlineAccountCredentials.Token = "";
		IdentityPtr->Login(0, OnlineAccountCredentials);
	}
}

void UEOSGameInstance::CreateSession(const FName& SessionName)
{
	if (SessionPtr)
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bAllowInvites = true;
		SessionSettings.bIsDedicated = false;
		SessionSettings.bIsLANMatch = false;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUseLobbiesIfAvailable = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.bAllowJoinInProgress = true;
		SessionSettings.bAllowJoinViaPresence = true;
		SessionSettings.NumPublicConnections = true;
		SessionSettings.NumPublicConnections = 10;

		//Passing an Arbitrary Data, and make it available to be read on the client.
		SessionSettings.Set(SessionNameKey, SessionName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		
		SessionPtr->CreateSession(0,"FunSession",SessionSettings);
	}
}

void UEOSGameInstance::FindSession()
{
	if (SessionPtr)
	{
		SessionSearch = MakeShareable(new FOnlineSessionSearch());

		SessionSearch->bIsLanQuery = false;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
		
		SessionPtr->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

FString UEOSGameInstance::GetSessionName(const FOnlineSessionSearchResult& SearchResult) const
{
	FString OutValue = {"Name None"};
	SearchResult.Session.SessionSettings.Get(SessionNameKey, OutValue);
	return OutValue;
}

void UEOSGameInstance::Init()
{
	Super::Init();
	OnlineSubsystem = IOnlineSubsystem::Get();
	IdentityPtr = OnlineSubsystem->GetIdentityInterface();
	IdentityPtr->OnLoginCompleteDelegates->AddUObject(this, &UEOSGameInstance::LoginComplete);

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
		UE_LOG(LogTemp, Warning, TEXT("Login successful!"));
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
	if (bWasSuccessful && SessionSearch->SearchResults.Num() > 0)
	{
		for (const FOnlineSessionSearchResult& LobbyFound : SessionSearch->SearchResults)
		{
			FString LobbyName = GetSessionName(LobbyFound);
			UE_LOG(LogTemp, Warning, TEXT("Found Session With id: %s"), *LobbyName);
		}
	
		SearchCompleted.Broadcast(SessionSearch->SearchResults);
	}
}

void UEOSGameInstance::JoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		FString TravelUrl;
		SessionPtr->GetResolvedConnectString("", TravelUrl);
		GetFirstLocalPlayerController(GetWorld())->ClientTravel(TravelUrl, ETravelType::TRAVEL_Absolute);
	}
}
