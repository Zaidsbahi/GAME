#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "EOSGameInstance.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSessionSearchCompleted, const TArray<FOnlineSessionSearchResult>&/*SearchResults*/);

UCLASS()
class GAME_API UEOSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	FOnSessionSearchCompleted SearchCompleted;

	UFUNCTION(BlueprintCallable)
	void Login();
	
	UFUNCTION(BlueprintCallable)
	void CreateSession(const FName& SessionName);

	UFUNCTION()
	void FindSession();
	FORCEINLINE FName GetSessionName() const {return SessionNameKey;}
	FString GetSessionName(const FOnlineSessionSearchResult& SearchResult) const;
	
protected:
	
	virtual void Init() override;

private:
	
	class IOnlineSubsystem* OnlineSubsystem;

	TSharedPtr<class IOnlineIdentity, ESPMode::ThreadSafe> IdentityPtr;
	TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> SessionPtr;
	
	void LoginComplete(int NumOfPlayers,bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
	void CreateSessionComplete(FName name, bool bWasSuccessful);
	void FindSessionComplete(bool bWasSuccessful);
	void JoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> GameLevel;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	const FName SessionNameKey{"SessionNameKey"};
	
};