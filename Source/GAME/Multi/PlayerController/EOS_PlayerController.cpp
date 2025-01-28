#include "EOS_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "GAME/Multi/GameInstance/EOS_GameInstance.h"
#include "TimerManager.h" // Include for timers

void AEOS_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Add the input mapping context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (InputMappingContext)
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
			UE_LOG(LogTemp, Log, TEXT("InputMappingContext added successfully"));
		}
	}
}

void AEOS_PlayerController::TogglePauseMenu()
{
	UE_LOG(LogTemp, Log, TEXT("TogglePauseMenu called"));

	if (!PauseMenuClass)
	{
		UE_LOG(LogTemp, Error, TEXT("PauseMenuClass is null! Assign it in the PlayerController Blueprint."));
		return;
	}

	if (!PauseMenuInstance)
	{
		PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
		if (PauseMenuInstance)
		{
			PauseMenuInstance->SetIsFocusable(true); // Ensure the widget is focusable
			UE_LOG(LogTemp, Log, TEXT("Pause menu widget created successfully"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create pause menu widget"));
			return;
		}
	}

	if (PauseMenuInstance->IsInViewport())
	{
		// Close the pause menu
		PauseMenuInstance->RemoveFromParent ();
		//SetPause(false);
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		UE_LOG(LogTemp, Log, TEXT("Pause menu closed"));
	}
	else
	{
		// Show the pause menu
		PauseMenuInstance->AddToViewport();
		//SetPause(true);
		bShowMouseCursor = true;

		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(PauseMenuInstance->TakeWidget());
		SetInputMode(InputMode);

		UE_LOG(LogTemp, Log, TEXT("Pause menu opened"));
	}
}

void AEOS_PlayerController::RemoveReadyWidget_Implementation()
{
	if (ReadyWidgetInstance)
	{
		ReadyWidgetInstance->RemoveFromParent();
		ReadyWidgetInstance = nullptr;

		// Hide mouse cursor and reset input mode to game only
		bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);

		UE_LOG(LogTemp, Log, TEXT("Ready-Up widget removed for player"));
	}
}


void AEOS_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (PauseInputAction)
		{
			EnhancedInputComponent->BindAction(PauseInputAction, ETriggerEvent::Triggered, this, &AEOS_PlayerController::TogglePauseMenu);
			UE_LOG(LogTemp, Log, TEXT("Pause action bound to input"));
		}
	}
}

void AEOS_PlayerController::OnNetCleanup(class UNetConnection* Connection)
{
	UEOS_GameInstance* GameInstanceRef = Cast<UEOS_GameInstance>(GetWorld()->GetGameInstance());
	if (GameInstanceRef)
	{
	GameInstanceRef->DestroySession();
	}
	
	
	Super::OnNetCleanup(Connection);
}


void AEOS_PlayerController::ShowReadyWidget_Implementation(TSubclassOf<UUserWidget> WidgetClass)
{
	if (WidgetClass && !ReadyWidgetInstance)
	{
		ReadyWidgetInstance = CreateWidget<UUserWidget>(this, WidgetClass);
		if (ReadyWidgetInstance)
		{
			ReadyWidgetInstance->AddToViewport();

			// Enable the mouse cursor and lock input to UI
			bShowMouseCursor = true;
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(ReadyWidgetInstance->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			SetInputMode(InputMode);

			UE_LOG(LogTemp, Log, TEXT("Ready-Up widget displayed successfully on client"));
		}
	}
}

void AEOS_PlayerController::DisableInputForPlayer_Implementation()
{
	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->DisableInput(this);
		UE_LOG(LogTemp, Log, TEXT("Player input disabled"));
	}
}

void AEOS_PlayerController::EnableInputForPlayer_Implementation()
{
	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->EnableInput(this);
	}

	// Hide mouse cursor and set input mode back to game
	bShowMouseCursor = false;
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	UE_LOG(LogTemp, Log, TEXT("Player input enabled"));
}