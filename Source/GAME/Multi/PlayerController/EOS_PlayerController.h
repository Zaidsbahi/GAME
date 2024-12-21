#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EOS_PlayerController.generated.h"


UCLASS()
class GAME_API AEOS_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void OnNetCleanup(class UNetConnection* Connection) override;
};
