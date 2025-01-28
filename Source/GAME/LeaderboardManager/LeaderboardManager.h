#pragma once

#include "CoreMinimal.h"
#include "eos_sdk.h"          
#include "eos_leaderboards.h"
#include "LeaderboardManager.generated.h"


UCLASS()
class GAME_API ULeaderboardManager : public UObject
{

	GENERATED_BODY()

public:
	
	ULeaderboardManager();

	// Function to submit a score
	UFUNCTION(BlueprintCallable, Category = "Leaderboard")
	void SubmitScore(FString PlayerName, int32 Score);

	static void EOS_CALL OnSubmitScoreComplete(const EOS_Stats_IngestStatCompleteCallbackInfo* Data);
	
	// Function to retrieve leaderboard rankings
	UFUNCTION(BlueprintCallable, Category = "Leaderboard")
	void FetchLeaderboard();
	static void EOS_CALL OnFetchLeaderboardComplete(const EOS_Leaderboards_OnQueryLeaderboardRanksCompleteCallbackInfo* Data);
	
	UFUNCTION()
	void LoginUser();

	UFUNCTION(BlueprintCallable)
	void FetchUserScore();

	UFUNCTION()
	void LogoutUser();

	
private:
	
	static EOS_HPlatform PlatformHandle;
	static EOS_ProductUserId LoggedInUserId;
	
	static void EOS_CALL OnLoginComplete(const EOS_Auth_LoginCallbackInfo* Data);
	
	// Callback function for leaderboard queries
	static void EOS_CALL OnStatIngestComplete(const EOS_Stats_OnIngestStatCompleteCallback* Data);

	// Helper function to handle leaderboard queries
	static void EOS_CALL OnQueryLeaderboardComplete(const EOS_Leaderboards_OnQueryLeaderboardUserScoresCompleteCallbackInfo* Data);


	static void EOS_CALL OnLogoutComplete(const EOS_Auth_LogoutCallbackInfo* Data);
	
};



