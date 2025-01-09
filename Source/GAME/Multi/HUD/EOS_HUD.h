#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "EOS_HUD.generated.h"


UCLASS()
class GAME_API AEOS_HUD : public AHUD
{
	GENERATED_BODY()

protected:

	// Reference to the timer widget class
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> TimerWidgetClass;

	// Reference to the timer widget
	UPROPERTY()
	class UUserWidget* TimerWidget;

public:
	// Function to initialize the HUD
	virtual void BeginPlay() override;
	
	
};
