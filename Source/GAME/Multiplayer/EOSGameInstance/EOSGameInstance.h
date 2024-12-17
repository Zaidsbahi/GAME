#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
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

protected:
	
	virtual void Init() override;

private:
	
	class IOnlineSubsystem* OnlineSubsystem;

	TSharedPtr<class IOnlineIdentity, ESPMode::ThreadSafe> IdentityPtr;
	TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> SessionPtr;
	
	void LoginComplete(int NumOfPlayers,bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
	void CreateSessionComplete(FName name, bool bWasSuccessful);

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> GameLevel;
	
};
