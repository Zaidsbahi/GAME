
#include "EOS_PlayerController.h"

#include "GAME/Multi/GameInstance/EOS_GameInstance.h"

void AEOS_PlayerController::OnNetCleanup(class UNetConnection* Connection)
{
	UEOS_GameInstance* GameInstanceRef = Cast<UEOS_GameInstance>(GetWorld()->GetGameInstance());
	if (GameInstanceRef)
	{
	GameInstanceRef->DestroySession();
	}
	
	
	Super::OnNetCleanup(Connection);
}
