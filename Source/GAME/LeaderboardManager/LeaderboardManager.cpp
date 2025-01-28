#include "LeaderboardManager.h"
#include "eos_sdk.h"
#include "CoreMinimal.h"
#include "eos_stats.h"
#include "eos_auth.h"
#include "eos_connect.h"
#include "eos_common.h"

EOS_HPlatform ULeaderboardManager::PlatformHandle = nullptr;
EOS_ProductUserId ULeaderboardManager::LoggedInUserId = nullptr;
ULeaderboardManager::ULeaderboardManager()
{
	
	EOS_InitializeOptions InitOptions = {};
	InitOptions.ApiVersion = EOS_INITIALIZE_API_LATEST;
	InitOptions.ProductName = "Mosiacrush Demo";
	InitOptions.ProductVersion = "1.0";

	EOS_EResult InitResult = EOS_Initialize(&InitOptions);
	if (InitResult != EOS_EResult::EOS_Success)
	{
		UE_LOG(LogTemp, Error, TEXT("EOS SDK Initialization Failed!"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("EOS SDK Initialized Successfully"));
	}

	EOS_Platform_Options PlatformOptions = {};
	PlatformOptions.ApiVersion = EOS_PLATFORM_OPTIONS_API_LATEST;
	PlatformOptions.ProductId = "7cf7ab96ff9244e598e649f354a4a2b9";
	PlatformOptions.SandboxId = "p-xtklmhw55y2z4x2qd68sees85tj8sj";
	PlatformOptions.ClientCredentials.ClientId = "xyza7891GCggUI6mqOcdN2p10VqTkZFG";
	PlatformOptions.ClientCredentials.ClientSecret = "xz1spSbx8DSVGlG3Zpr7we2sCb5AWdS89/Rz11Be4yg";
	PlatformOptions.DeploymentId = "28217464dfe94e638a6b9361e3e02367";
	PlatformOptions.bIsServer = EOS_FALSE;
	PlatformOptions.EncryptionKey = "your-encryption-key";
	PlatformOptions.OverrideCountryCode = nullptr;
	PlatformOptions.OverrideLocaleCode = nullptr;

	PlatformHandle = EOS_Platform_Create(&PlatformOptions);
	if (!PlatformHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("EOS Platform creation failed!"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("EOS Platform created successfully"));
	}
}
void ULeaderboardManager::LoginUser()
{
	EOS_Auth_Credentials Credentials = {};
	Credentials.ApiVersion = EOS_AUTH_CREDENTIALS_API_LATEST;
	Credentials.Type = EOS_ELoginCredentialType::EOS_LCT_PersistentAuth; // Or EOS_LCT_PersistentAuth for automatic login
	Credentials.Id = nullptr; // Not required for account portal
	Credentials.Token = nullptr;

	EOS_Auth_LoginOptions LoginOptions = {};
	LoginOptions.ApiVersion = EOS_AUTH_LOGIN_API_LATEST;
	LoginOptions.ScopeFlags = EOS_EAuthScopeFlags::EOS_AS_BasicProfile;
	LoginOptions.Credentials = &Credentials;

	EOS_HAuth AuthHandle = EOS_Platform_GetAuthInterface(PlatformHandle);
	EOS_Auth_Login(AuthHandle, &LoginOptions, nullptr, [](const EOS_Auth_LoginCallbackInfo* Data)
	{
		if (Data->ResultCode == EOS_EResult::EOS_Success)
		{
			UE_LOG(LogTemp, Log, TEXT("User successfully logged in!"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to login: %s"), *FString(EOS_EResult_ToString(Data->ResultCode)));
		}
	});
	
	EOS_Auth_Login(AuthHandle, &LoginOptions, nullptr, &ULeaderboardManager::OnLoginComplete);

}

void ULeaderboardManager::FetchUserScore()
{
	if (!PlatformHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("PlatformHandle is null!"));
		return;
	}

	EOS_HLeaderboards LeaderboardsHandle = EOS_Platform_GetLeaderboardsInterface(PlatformHandle);

	EOS_Leaderboards_QueryLeaderboardUserScoresOptions QueryUserOptions = {};
	QueryUserOptions.ApiVersion = EOS_LEADERBOARDS_QUERYLEADERBOARDUSERSCORES_API_LATEST;
	QueryUserOptions.UserIds = &LoggedInUserId;
	QueryUserOptions.UserIdsCount = 1;
	QueryUserOptions.StartTime = EOS_LEADERBOARDS_TIME_UNDEFINED;
	QueryUserOptions.EndTime = EOS_LEADERBOARDS_TIME_UNDEFINED;

	EOS_Leaderboards_UserScoresQueryStatInfo StatInfo = {};
	StatInfo.ApiVersion = EOS_LEADERBOARDS_USERSCORESQUERYSTATINFO_API_LATEST;
	StatInfo.StatName = "Track1_Time";
	StatInfo.Aggregation = EOS_ELeaderboardAggregation::EOS_LA_Sum; 

	QueryUserOptions.StatInfo = &StatInfo;
	QueryUserOptions.StatInfoCount = 1;

	EOS_Leaderboards_QueryLeaderboardUserScores(LeaderboardsHandle, &QueryUserOptions, nullptr, &ULeaderboardManager::OnQueryLeaderboardComplete);

}

void ULeaderboardManager::LogoutUser()
{
	if (!LoggedInUserId)
	{
		UE_LOG(LogTemp, Warning, TEXT("User not logged in."));
		return;
	}

	EOS_Auth_LogoutOptions LogoutOptions = {};
	LogoutOptions.ApiVersion = EOS_AUTH_LOGOUT_API_LATEST;
	//LogoutOptions.LocalUserId = LoggedInUserId;

	EOS_HAuth AuthHandle = EOS_Platform_GetAuthInterface(PlatformHandle);
	EOS_Auth_Logout(AuthHandle, &LogoutOptions, nullptr, &ULeaderboardManager::OnLogoutComplete);
}

void ULeaderboardManager::OnLogoutComplete(const EOS_Auth_LogoutCallbackInfo* Data)
{
	if (Data->ResultCode == EOS_EResult::EOS_Success)
	{
		LoggedInUserId = nullptr;
		UE_LOG(LogTemp, Log, TEXT("User successfully logged out!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to log out: %s"), *FString(EOS_EResult_ToString(Data->ResultCode)));
	}
}


void ULeaderboardManager::OnLoginComplete(const EOS_Auth_LoginCallbackInfo* Data)
{
	if (Data->ResultCode == EOS_EResult::EOS_Success)
	{
		UE_LOG(LogTemp, Log, TEXT("User successfully logged in!"));

		EOS_HConnect ConnectHandle = EOS_Platform_GetConnectInterface(PlatformHandle);

		EOS_Connect_LinkAccountOptions LinkAccountOptions = {};
		LinkAccountOptions.ApiVersion = EOS_CONNECT_LINKACCOUNT_API_LATEST;
		//LinkAccountOptions.LocalUserId = Data->LocalUserId;  // Epic Account ID from login callback
		LinkAccountOptions.ContinuanceToken = nullptr;
		//LinkAccountOptions.bPreferredLocal = EOS_TRUE;

		EOS_Connect_LinkAccount(ConnectHandle, &LinkAccountOptions, nullptr, [](const EOS_Connect_LinkAccountCallbackInfo* CallbackData)
		{
			if (CallbackData->ResultCode == EOS_EResult::EOS_Success)
			{
				LoggedInUserId = CallbackData->LocalUserId;  // This will now be a valid EOS_ProductUserId
				UE_LOG(LogTemp, Log, TEXT("Successfully linked Epic Account ID to Product User ID."));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to link account: %s"), *FString(EOS_EResult_ToString(CallbackData->ResultCode)));
			}
		});
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to login: %s"), *FString(EOS_EResult_ToString(Data->ResultCode)));
	}

}
void ULeaderboardManager::SubmitScore(FString PlayerName, int32 Score)
{
	if (!PlatformHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("PlatformHandle is null!"));
		return;
	}

	EOS_HStats StatsHandle = EOS_Platform_GetStatsInterface(PlatformHandle);

	EOS_Stats_IngestData IngestData;
	IngestData.ApiVersion = EOS_STATS_INGESTDATA_API_LATEST;
	IngestData.StatName = "Track1_Time";  // Your leaderboard stat name
	IngestData.IngestAmount = Score;    // Set the player's score

	EOS_Stats_IngestStatOptions IngestOptions = {};
	IngestOptions.ApiVersion = EOS_STATS_INGESTSTAT_API_LATEST;
	IngestOptions.LocalUserId = LoggedInUserId;  // You need to store the logged-in user ID
	IngestOptions.TargetUserId = LoggedInUserId;
	IngestOptions.Stats = &IngestData;
	IngestOptions.StatsCount = 1;

	EOS_Stats_IngestStat(StatsHandle, &IngestOptions, nullptr, &ULeaderboardManager::OnSubmitScoreComplete);

}
void ULeaderboardManager::OnSubmitScoreComplete(const EOS_Stats_IngestStatCompleteCallbackInfo* Data)
{
	if (Data->ResultCode == EOS_EResult::EOS_Success)
	{
		UE_LOG(LogTemp, Log, TEXT("Score successfully submitted!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to submit score: %s"), *FString(EOS_EResult_ToString(Data->ResultCode)));
	}
}
void ULeaderboardManager::FetchLeaderboard()
{
	if (!PlatformHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("PlatformHandle is null!"));
		return;
	}

	EOS_HLeaderboards LeaderboardsHandle = EOS_Platform_GetLeaderboardsInterface(PlatformHandle);

	EOS_Leaderboards_QueryLeaderboardRanksOptions QueryOptions = {};
	QueryOptions.ApiVersion = EOS_LEADERBOARDS_QUERYLEADERBOARDRANKS_API_LATEST;
	QueryOptions.LeaderboardId = "Track1_Ranking";  // Your leaderboard ID
	QueryOptions.LocalUserId = LoggedInUserId;      // The logged-in user

	EOS_Leaderboards_QueryLeaderboardRanks(LeaderboardsHandle, &QueryOptions, nullptr, &ULeaderboardManager::OnFetchLeaderboardComplete);
}
void ULeaderboardManager::OnFetchLeaderboardComplete(
	const EOS_Leaderboards_OnQueryLeaderboardRanksCompleteCallbackInfo* Data)
{
	if (Data->ResultCode == EOS_EResult::EOS_Success)
	{
		UE_LOG(LogTemp, Log, TEXT("Leaderboard data fetched successfully!"));

		EOS_HLeaderboards LeaderboardsHandle = EOS_Platform_GetLeaderboardsInterface(PlatformHandle);

		uint32_t RecordCount = EOS_Leaderboards_GetLeaderboardRecordCount(LeaderboardsHandle, nullptr);

		for (uint32_t Index = 0; Index < RecordCount; Index++)
		{
			EOS_Leaderboards_CopyLeaderboardRecordByIndexOptions CopyOptions = {};
			CopyOptions.ApiVersion = EOS_LEADERBOARDS_COPYLEADERBOARDRECORDBYINDEX_API_LATEST;
			CopyOptions.LeaderboardRecordIndex = Index;

			EOS_Leaderboards_LeaderboardRecord* LeaderboardRecord = nullptr;
			if (EOS_Leaderboards_CopyLeaderboardRecordByIndex(LeaderboardsHandle, &CopyOptions, &LeaderboardRecord) == EOS_EResult::EOS_Success)
			{
				char UserIdBuffer[EOS_PRODUCTUSERID_MAX_LENGTH + 1];
				int32_t BufferSize = sizeof(UserIdBuffer);

				EOS_EResult Result = EOS_ProductUserId_ToString(LeaderboardRecord->UserId, UserIdBuffer, &BufferSize);

				if (Result == EOS_EResult::EOS_Success)
				{
					FString UserId = FString(ANSI_TO_TCHAR(UserIdBuffer));
					UE_LOG(LogTemp, Log, TEXT("User ID: %s"), *UserId);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to convert ProductUserId to string"));
				}
				int32 Score = LeaderboardRecord->Score;
				int32 Rank = LeaderboardRecord->Rank;
				EOS_Leaderboards_LeaderboardRecord_Release(LeaderboardRecord);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to fetch leaderboard data: %s"), *FString(EOS_EResult_ToString(Data->ResultCode)));
	}
}


void ULeaderboardManager::OnStatIngestComplete(const EOS_Stats_OnIngestStatCompleteCallback* Data)
{

}

void ULeaderboardManager::OnQueryLeaderboardComplete(
	const EOS_Leaderboards_OnQueryLeaderboardUserScoresCompleteCallbackInfo* Data)
{
	if (Data->ResultCode == EOS_EResult::EOS_Success)
	{
		UE_LOG(LogTemp, Log, TEXT("User's leaderboard score fetched successfully!"));

		EOS_HLeaderboards LeaderboardsHandle = EOS_Platform_GetLeaderboardsInterface(PlatformHandle);

		EOS_Leaderboards_CopyLeaderboardUserScoreByIndexOptions CopyUserOptions = {};
		CopyUserOptions.ApiVersion = EOS_LEADERBOARDS_COPYLEADERBOARDUSERSCOREBYINDEX_API_LATEST;
		CopyUserOptions.LeaderboardUserScoreIndex = 0;

		EOS_Leaderboards_LeaderboardUserScore* UserScore = nullptr;
		if (EOS_Leaderboards_CopyLeaderboardUserScoreByIndex(LeaderboardsHandle, &CopyUserOptions, &UserScore) == EOS_EResult::EOS_Success)
		{
			int32 Score = UserScore->Score;
			UE_LOG(LogTemp, Log, TEXT("Your Score: %d"), Score);
			EOS_Leaderboards_LeaderboardUserScore_Release(UserScore);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to fetch user score: %s"), *FString(EOS_EResult_ToString(Data->ResultCode)));
	}

}

