#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "EOS_GameInstance.generated.h"

UCLASS()
class GAME_API UEOS_GameInstance : public UGameInstance
{
	
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category= "EOS Function")
	void LoginWithEOS(FString ID, FString Token, FString LoginType);
	
	UFUNCTION(BlueprintCallable,BlueprintPure, Category= "EOS Function")
	FString GetPlayerUsername();

	UFUNCTION(BlueprintCallable,BlueprintPure, Category= "EOS Function")
	bool IsPlayerLoggedIn();
	
	UFUNCTION(BlueprintCallable, Category= "EOS Function")
	void CreateEOSSession(bool bIsDeticatedServer, bool bIsLanServer, int32 NumberOfPublicConnections);
	
	UFUNCTION(BlueprintCallable, Category= "EOS Function")
	void FindSessionAndJoin();
	
	UFUNCTION(BluePrintCallable, Category= "EOS Function")
	void JoinSession();

	UFUNCTION(BluePrintCallable, Category= "EOS Function")
	void DestroySession();

	/////////////////////////
	/// Anonymously Login ///
	/////////////////////////

	UFUNCTION(BlueprintCallable, Category="EOS")
	void LoginAnonymously(FString Username);
	UFUNCTION(BlueprintCallable, Category="EOS")
	FString GetCurrentUsername();
	//void OnLoginComplete(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserID, const FString& Error);
	FString PlayerUsername;


	// Second Try
	void OnLoginComplete(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserID, const FString& Error);
	FString AnonymousUsername = TEXT("Player_") + FGuid::NewGuid().ToString();
	///////////////////////////////
	///////////////////////////////
	///////////////////////////////

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "EOS Variables")
	FString OpenLevelText;
	
	void LoginWithEOS_Return(int32 LocalUserNum, bool bWasSuccess, const FUniqueNetId& UserID, const FString& Error);
	void OnCreateSessionCompleted(FName SessionName, bool bWasSuccessful);
	void OnDestroySessionCompleted(FName SessionName, bool bWasSuccessful);
	void OnFindSessionCompleted(bool bWasSuccess);
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	// In your GameInstance class (e.g., UEOS_GameInstance):
	int32 CurrentTrackIndex = 1;
	
	FOnlineSessionSettings SessionSettings;
	
};
