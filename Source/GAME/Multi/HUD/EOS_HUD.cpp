#include "EOS_HUD.h"
#include "Blueprint/UserWidget.h"



void AEOS_HUD::BeginPlay()
{
	Super::BeginPlay();

	// Check if the TimerWidgetClass is valid
	if (TimerWidgetClass)
	{
		TimerWidget = CreateWidget<UUserWidget>(GetWorld(), TimerWidgetClass);
		if (TimerWidget)
		{
			TimerWidget->AddToViewport();
			UE_LOG(LogTemp, Log, TEXT("Timer widget successfully added to viewport."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("TimerWidgetClass is not set in the HUD."));
	}
}
