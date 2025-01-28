#include "EOS_GameInstance.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Online/OnlineSessionNames.h"
#include "OnlineSessionSettings.h"

void UEOS_GameInstance::LoginWithEOS(FString ID, FString Token, FString LoginType)
{
	IOnlineSubsystem *SubSystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubSystemRef)
	{
		IOnlineIdentityPtr IdentityPtrRef = SubSystemRef->GetIdentityInterface();
		if (IdentityPtrRef)
		{
			FOnlineAccountCredentials AccountDetails;
			//AccountDetails.Id = ID;
			//AccountDetails.Token = Token;
			//AccountDetails.Type = LoginType;
			AccountDetails.Id = TEXT(""); // No need to specify user ID for persistent auth
			AccountDetails.Token = TEXT(""); // Token will be retrieved internally
			AccountDetails.Type = TEXT("persistentauth"); // Use persistent authentication
			IdentityPtrRef->OnLoginCompleteDelegates->AddUObject(this, &UEOS_GameInstance::LoginWithEOS_Return);
			IdentityPtrRef->Login(0, AccountDetails);
		}
	}
}

FString UEOS_GameInstance::GetPlayerUsername()
{
	IOnlineSubsystem *SubSystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubSystemRef)
	{
		IOnlineIdentityPtr IdentityPtrRef = SubSystemRef->GetIdentityInterface();
		if (IdentityPtrRef)
		{
			if (IdentityPtrRef->GetLoginStatus(0) == ELoginStatus::LoggedIn)
			{
				return IdentityPtrRef->GetPlayerNickname(0);
			}
		}
	}
	return FString();
}

bool UEOS_GameInstance::IsPlayerLoggedIn()
{
	IOnlineSubsystem *SubSystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubSystemRef)
	{
		IOnlineIdentityPtr IdentityPtrRef = SubSystemRef->GetIdentityInterface();
		if (IdentityPtrRef)
		{
			if (IdentityPtrRef->GetLoginStatus(0) == ELoginStatus::LoggedIn)
			{
				return true;
			}
		}
	}
	return false;
}

void UEOS_GameInstance::LoginWithEOS_Return(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserID,
                                            const FString& Error)
{
	if (bWasSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("Auto-login successful!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Login with EOS Failed Reason - %s"), *Error);
		//LoginWithEOSManual();  // Fallback to manual login
	}
}
void UEOS_GameInstance::OnCreateSessionCompleted(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		GetWorld()->ServerTravel(OpenLevelText);
	}
}

void UEOS_GameInstance::OnDestroySessionCompleted(FName SessionName, bool bWasSuccessful)
{
}

void UEOS_GameInstance::OnFindSessionCompleted(bool bWasSuccess)
{
	if (bWasSuccess)
	{
		IOnlineSubsystem *SubSystemRef = Online::GetSubsystem(this->GetWorld());
		if (SubSystemRef)
		{
			IOnlineSessionPtr SessionPtrRef = SubSystemRef->GetSessionInterface();
			if (SessionPtrRef)
			{
				if (SessionSearch->SearchResults.Num() > 0)
				{
					SessionPtrRef->OnJoinSessionCompleteDelegates.AddUObject(this, &UEOS_GameInstance::OnJoinSessionCompleted);
					SessionPtrRef->JoinSession(0, FName("MainSession"), SessionSearch->SearchResults[0]);
				}
				else
				{
					CreateEOSSession(false,false,2);
				}
			}
		}
	}
	else
	{
		CreateEOSSession(false, false, 2);
	}
}

void UEOS_GameInstance::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		if (APlayerController* PlayerControllerRef = UGameplayStatics::GetPlayerController(GetWorld(),0))
		{
			FString JoinAddress;
			IOnlineSubsystem *SubSystemRef = Online::GetSubsystem(this->GetWorld());
			if (SubSystemRef)
			{
				IOnlineSessionPtr SessionPtrRef = SubSystemRef->GetSessionInterface();
				if (SessionPtrRef)
				{
					SessionPtrRef->GetResolvedConnectString(FName("MainSession"), JoinAddress);
					UE_LOG(LogTemp, Warning, TEXT("Join Address is %s"), *JoinAddress);
					if (!JoinAddress.IsEmpty())
					{
						PlayerControllerRef->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
					}
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to join session."));
	}
}

void UEOS_GameInstance::CreateEOSSession(bool bIsDeticatedServer, bool bIsLanServer, int32 NumberOfPublicConnections)
{
	IOnlineSubsystem *SubSystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubSystemRef)
	{
		IOnlineSessionPtr SessionPtrRef = SubSystemRef->GetSessionInterface();
		if (SessionPtrRef)
		{
			FOnlineSessionSettings SessionCreationInfo;
			SessionCreationInfo.bIsDedicated = bIsDeticatedServer;
			SessionCreationInfo.bAllowInvites = true;
			SessionCreationInfo.bIsLANMatch = bIsLanServer;
			SessionCreationInfo.NumPublicConnections = NumberOfPublicConnections;
			SessionCreationInfo.bUseLobbiesIfAvailable = false;
			SessionCreationInfo.bUsesPresence = false;
			SessionCreationInfo.bShouldAdvertise = true;
			SessionCreationInfo.Set(SEARCH_KEYWORDS, FString("RandomHi"), EOnlineDataAdvertisementType::ViaOnlineService);
			SessionPtrRef->OnCreateSessionCompleteDelegates.AddUObject(this, &UEOS_GameInstance::OnCreateSessionCompleted);
			SessionPtrRef->CreateSession(0, FName("MainSession"), SessionCreationInfo);
		}
	}
}

void UEOS_GameInstance::FindSessionAndJoin()
{
	IOnlineSubsystem *SubSystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubSystemRef)
	{
		IOnlineSessionPtr SessionPtrRef = SubSystemRef->GetSessionInterface();
		if (SessionPtrRef)
		{
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			SessionSearch->bIsLanQuery = false;
			SessionSearch->MaxSearchResults = 2;
			SessionSearch->QuerySettings.SearchParams.Empty();
			SessionPtrRef->OnFindSessionsCompleteDelegates.AddUObject(this, &UEOS_GameInstance::OnFindSessionCompleted);
			SessionPtrRef->FindSessions(0, SessionSearch.ToSharedRef());
		}
	}
}


void UEOS_GameInstance::JoinSession()
{
}

void UEOS_GameInstance::DestroySession()
{
	
}
// Anonymous Login
void UEOS_GameInstance::LoginAnonymously(FString Username)
{
	IOnlineSubsystem *SubSystemRef = Online::GetSubsystem(this->GetWorld());
	if (SubSystemRef)
	{
		IOnlineIdentityPtr IdentityPtrRef = SubSystemRef->GetIdentityInterface();
		if (IdentityPtrRef)
		{
			// Store the username for later use
			PlayerUsername = Username;
            
			FOnlineAccountCredentials AccountDetails;
			AccountDetails.Type = TEXT("anonymous");  // Use the anonymous login type
			AccountDetails.Id = Username;             // Custom username provided by the player
			AccountDetails.Token = TEXT("");          

			UE_LOG(LogTemp, Log, TEXT("Attempting Anonymous Login with Username: %s"), *Username);
            
			IdentityPtrRef->OnLoginCompleteDelegates->AddUObject(this, &UEOS_GameInstance::OnLoginComplete);
			IdentityPtrRef->Login(0, AccountDetails);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to get Identity Interface."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get Online Subsystem."));
	}
}
void UEOS_GameInstance::OnLoginComplete(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserID, const FString& Error)
{
	if (bWasSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("Auto Login Successful - Player ID: %s"), *UserID.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Auto Login Failed: %s"), *Error);
	}
}
FString UEOS_GameInstance::GetCurrentUsername()
{
	return PlayerUsername.IsEmpty() ? TEXT("Guest") : PlayerUsername;
}







