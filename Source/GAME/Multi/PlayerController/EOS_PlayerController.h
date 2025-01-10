#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EOS_PlayerController.generated.h"


UCLASS()
class GAME_API AEOS_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();

protected:

	// Enhanced Input Subsystem
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<class UInputAction> PauseInputAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> PauseMenuClass;

	UPROPERTY()
	class UUserWidget* PauseMenuInstance;

	virtual void SetupInputComponent() override;
	
	virtual void OnNetCleanup(class UNetConnection* Connection) override;
};
