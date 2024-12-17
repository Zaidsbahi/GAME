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
protected:
	virtual void Init() override;

private:
	class IOnlineSubsystem* OnlineSubsystem;
	TSharedPtr<class IOnlineIdentity, ESPMode::ThreadSafe> IdentityPtr;

	void LoginComplete(int NumOfPlayers,bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
	
};
