#include "ReadyWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GAME/PlayerState/PlayerState_Base.h"
#include "GameFramework/PlayerController.h"

void UReadyWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ReadyButton)
    {
        ReadyButton->OnClicked.AddDynamic(this, &UReadyWidget::OnReadyClicked);
    }
}

void UReadyWidget::OnReadyClicked()
{
    if (APlayerController* PC = GetOwningPlayer())
    {
        if (APlayerState_Base* PlayerState = PC->GetPlayerState<APlayerState_Base>())
        {
            PlayerState->SetReady(true);

            // Update UI
            if (StatusText)
            {
                StatusText->SetText(FText::FromString("Ready! Waiting for others..."));
            }

            ReadyButton->SetIsEnabled(false); // Disable button
        }
    }
}