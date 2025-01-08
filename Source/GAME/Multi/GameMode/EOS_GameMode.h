#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EOS_GameMode.generated.h"


UCLASS()
class GAME_API AEOS_GameMode : public AGameModeBase
{
	GENERATED_BODY()

	//AEOS_GameMode();

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION(BlueprintCallable, Category = "Game")
	void RestartCurrentLevel();
	
};
