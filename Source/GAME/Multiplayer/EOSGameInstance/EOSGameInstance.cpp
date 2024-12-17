#include "EOSGameInstance.h"
#include "OnlineSubSystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"

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

void UEOSGameInstance::CreateSession()
{
	if (SessionPtr)
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bAllowInvites = true;
		SessionSettings.bIsDedicated = false;
		SessionSettings.bIsLANMatch = false;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.bAllowJoinInProgress = true;
		SessionSettings.bAllowJoinViaPresence = true;
		SessionSettings.NumPublicConnections = true;

		//Passing an Arbitrary Data, and make it available to be read on the client.
		SessionSettings.Set(FName("LobbyName"), FString("MyFunLobby"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		
		SessionPtr->CreateSession(0,"FunSession",SessionSettings);
	}
}

void UEOSGameInstance::Init()
{
	Super::Init();
	OnlineSubsystem = IOnlineSubsystem::Get();
	IdentityPtr = OnlineSubsystem->GetIdentityInterface();
	IdentityPtr->OnLoginCompleteDelegates->AddUObject(this, &UEOSGameInstance::LoginComplete);

	SessionPtr = OnlineSubsystem->GetSessionInterface();
	SessionPtr->OnCreateSessionCompleteDelegates.AddUObject(this, &UEOSGameInstance::CreateSessionComplete);
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
