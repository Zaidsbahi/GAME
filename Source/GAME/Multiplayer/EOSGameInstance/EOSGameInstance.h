#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "EOSGameInstance.generated.h"

UCLASS()
class GAME_API UEOSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void Login();
	
	UFUNCTION(BlueprintCallable)
	void CreateSession();

	UFUNCTION(BlueprintCallable)
	void FindSession();
	
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
	
};