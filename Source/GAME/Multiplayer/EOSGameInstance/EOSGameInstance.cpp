#include "EOSGameInstance.h"
#include "OnlineSubSystem.h"
#include "Interfaces/OnlineIdentityInterface.h"

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

void UEOSGameInstance::Init()
{
	Super::Init();
	OnlineSubsystem = IOnlineSubsystem::Get();
	IdentityPtr = OnlineSubsystem->GetIdentityInterface();
	IdentityPtr->OnLoginCompleteDelegates->AddUObject(this, &UEOSGameInstance::LoginComplete);
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
