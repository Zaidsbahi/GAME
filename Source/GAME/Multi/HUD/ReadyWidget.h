#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReadyWidget.generated.h"

UCLASS()
class GAME_API UReadyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	// Bind a text to show the "waiting" status
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* StatusText;
	UPROPERTY(meta = (BindWidget))
	class UButton* ReadyButton;

	UFUNCTION()
	void OnReadyClicked();
	
	virtual void NativeConstruct() override;
	
};